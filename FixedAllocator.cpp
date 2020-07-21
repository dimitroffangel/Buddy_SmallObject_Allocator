#include "FixedAllocator.h"

#include <vector>
#include <cassert>

FixedAllocator::FixedAllocator(std::size_t blockSize)
	:m_BlockSize(blockSize)
{
	assert(m_BlockSize > 0);

	m_PreviousFixedAlloc = m_NextFixedAlloc = this;

	std::size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;

	if (numBlocks > UNSIGNED_CHAR_MAX)
		numBlocks = UNSIGNED_CHAR_MAX;

	else if (numBlocks == 0)
	{
		numBlocks = DefaultNumberOfBlocks * blockSize;
	}

	m_NumberOfBlocks = static_cast<unsigned char>(numBlocks);
	assert(m_NumberOfBlocks == numBlocks);
}

FixedAllocator::~FixedAllocator()
{
	if (m_PreviousFixedAlloc != this)
	{
		m_PreviousFixedAlloc->m_NextFixedAlloc = m_NextFixedAlloc;
		m_NextFixedAlloc->m_PreviousFixedAlloc = m_PreviousFixedAlloc;
		return;
	}

	assert(m_PreviousFixedAlloc == m_NextFixedAlloc);

	Chunks::iterator it = m_Chunks.begin();

	for (; it != m_Chunks.end(); ++it)
	{
		assert(it->m_BlockAvailable == m_NumberOfBlocks);
		it->Release();
	}
}

FixedAllocator::FixedAllocator(const FixedAllocator& other)
{
	m_NumberOfBlocks = other.m_NumberOfBlocks;

	m_Chunks = std::vector<Chunk>(other.m_Chunks.begin(), other.m_Chunks.end());

	m_RecentlyAllocatedChunk = other.m_RecentlyAllocatedChunk;
	m_RecentlyDeallocatedChunk = other.m_RecentlyDeallocatedChunk;

	m_PreviousFixedAlloc = other.m_PreviousFixedAlloc;
	m_NextFixedAlloc = other.m_NextFixedAlloc;
}

FixedAllocator& FixedAllocator::operator=(const FixedAllocator& rhs)
{
	if (this != &rhs)
	{
		for (auto& chunk : m_Chunks)
		{
			chunk.Release();
		}

		m_NumberOfBlocks = rhs.m_NumberOfBlocks;

		m_Chunks = std::vector<Chunk>(rhs.m_Chunks.begin(), rhs.m_Chunks.end());

		m_RecentlyAllocatedChunk = rhs.m_RecentlyAllocatedChunk;
		m_RecentlyDeallocatedChunk = rhs.m_RecentlyDeallocatedChunk;

		m_PreviousFixedAlloc = rhs.m_PreviousFixedAlloc;
		m_NextFixedAlloc = rhs.m_NextFixedAlloc;
	}

	return *this;
}



void FixedAllocator::DoDeallocation(void* pointer)
{
	// that holds the nullptr check
	assert(m_RecentlyDeallocatedChunk->m_PointerToData <= pointer);
	assert(m_RecentlyDeallocatedChunk->m_PointerToData + (m_NumberOfBlocks * m_BlockSize) > pointer);

	m_RecentlyDeallocatedChunk->Deallocate(pointer, m_BlockSize);

	if (m_RecentlyDeallocatedChunk->m_BlockAvailable == m_NumberOfBlocks)
	{
		Chunk& lastChunk = m_Chunks.back();

		if (&lastChunk == m_RecentlyDeallocatedChunk)
		{
			if ((m_Chunks.size() == 2 && m_Chunks[0].m_BlockAvailable == m_NumberOfBlocks) ||
				m_Chunks.size() > 1)
			{
				lastChunk.Release();
				m_Chunks.pop_back();

				// m_RecentlyAllocatedChunk is pointing to sumething invalid
				m_RecentlyAllocatedChunk = m_RecentlyDeallocatedChunk = &m_Chunks.front();
			}

			return;
		}

		if (lastChunk.m_BlockAvailable == m_NumberOfBlocks)
		{
			lastChunk.Release();
			m_Chunks.pop_back();

			// should not be used
			m_RecentlyAllocatedChunk = m_RecentlyDeallocatedChunk = &m_Chunks.front();
		}

		else
		{
			// when swapping what happens with the address ??
			std::swap(*m_RecentlyDeallocatedChunk, lastChunk);
			m_Chunks.pop_back();

			// should not be used
			m_RecentlyDeallocatedChunk = &m_Chunks.back();
		}
	}
}

Chunk* FixedAllocator::FindChunkWithPointer(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedAllocator::FindChunkWithPointer argument is nullptr " << '\n';
	}

	assert(!m_Chunks.empty());
	assert(m_RecentlyDeallocatedChunk != nullptr);

	const std::size_t chunkLength = m_NumberOfBlocks * m_BlockSize;

	Chunk* low = m_RecentlyDeallocatedChunk;
	Chunk* high = m_RecentlyDeallocatedChunk + 1;
	Chunk* lowBound = &m_Chunks.front();
	Chunk* highBound = &m_Chunks.back() + 1;

	if (high == highBound)
		high = nullptr;

	for (;;)
	{
		if (low)
		{
			if (pointer >= low->m_PointerToData && pointer < low->m_PointerToData + chunkLength)
			{
				return low;
			}

			if (low == lowBound)
			{
				low = nullptr;

				if (high == nullptr)
				{
					break;
				}
			}

			else
			{
				--low;
			}
		}

		if (high)
		{
			if (pointer >= high->m_PointerToData && pointer < high->m_PointerToData + chunkLength)
			{
				return high;
			}

			if (++high == highBound)
			{
				high = nullptr;

				if (low == nullptr)
				{
					break;
				}
			}
		}
	}

	// should not come to this
	assert(false);
	return nullptr;
}

void* FixedAllocator::Allocate()
{
	if (m_RecentlyAllocatedChunk == nullptr ||
		m_RecentlyAllocatedChunk->m_BlockAvailable == 0)
	{
		Chunks::iterator it = m_Chunks.begin();

		for (;; ++it)
		{
			if (it == m_Chunks.end())
			{
				//m_Chunks.reserve(m_Chunks.size() + 1);
				Chunk newChunk;
				newChunk.Initialize(m_BlockSize, m_NumberOfBlocks);
				m_Chunks.push_back(newChunk);
				m_RecentlyAllocatedChunk = &m_Chunks.back();
				m_RecentlyDeallocatedChunk = &m_Chunks.front();
				break;
			}

			if (it->m_BlockAvailable > 0)
			{
				m_RecentlyAllocatedChunk = &(*it);
				break;
			}
		}
	}

	assert(m_RecentlyAllocatedChunk != nullptr);
	assert(m_RecentlyAllocatedChunk->m_BlockAvailable > 0);

	return m_RecentlyAllocatedChunk->Allocate(m_BlockSize);
}

void FixedAllocator::Deallocate(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedAllocator::Dealloacate() is nullptr" << '\n';
		return;
	}

	assert(!m_Chunks.empty());
	assert(&m_Chunks.front() <= m_RecentlyDeallocatedChunk);
	assert(&m_Chunks.back() >= m_RecentlyDeallocatedChunk);

	m_RecentlyDeallocatedChunk = FindChunkWithPointer(pointer);
	assert(m_RecentlyDeallocatedChunk);

	DoDeallocation(pointer);
}
