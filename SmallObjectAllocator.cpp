#include "SmallObjectAllocator.h"

#include <cassert>

SmallObjectAllocator::SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
	:
	m_ChunkSize(chunkSize),
	m_MaxObjectSize(maxObjectSize)
{

}

void* SmallObjectAllocator::Allocate(std::size_t numberOfBytes)
{
	if (numberOfBytes > m_MaxObjectSize)
	{
		std::cout << "Object too big, will use the default new..." << '\n';
		return operator new(numberOfBytes);
	}

	if (m_PointerToLastAllocator != nullptr && m_PointerToLastAllocator->BlockSize() == numberOfBytes)
	{
		return m_PointerToLastAllocator->Allocate();
	}

	Pool::iterator it;

	for (it = m_Pool.begin(); it != m_Pool.end(); ++it)
	{
		if (it->BlockSize() == numberOfBytes)
		{
			break;
		}
	}

	if (it == m_Pool.end())
	{
		it = m_Pool.insert(it, std::move(FixedAllocator(numberOfBytes)));
		m_PointerToLastDeallocator = &(*m_Pool.begin());
		//m_PointerToLastDeallocator += sizeof(FixedAllocator);
	}

	m_PointerToLastAllocator = &(*it);
	return m_PointerToLastAllocator->Allocate();
}

void SmallObjectAllocator::Deallocate(void* pointer, std::size_t numberOfBytes)
{
	if (pointer == nullptr)
	{
		std::cerr << "SmallObjectAllocator::Deallocate pointer is nullptr " << '\n';
		return;
	}

	if (numberOfBytes > m_MaxObjectSize)
	{
		std::cerr << "Object too large, it cannot be here..." << '\n';
		return operator delete(pointer);
	}

	if (m_PointerToLastDeallocator != nullptr && m_PointerToLastDeallocator->BlockSize() == numberOfBytes)
	{
		m_PointerToLastDeallocator->Deallocate(pointer);
		return;
	}

	Pool::iterator it;

	for (it = m_Pool.begin(); it != m_Pool.end(); ++it)
	{
		if (it->BlockSize() == numberOfBytes)
		{
			break;
		}
	}

	assert(it != m_Pool.end());
	assert(it->BlockSize() == numberOfBytes);

	m_PointerToLastDeallocator = &(*it);
	m_PointerToLastDeallocator->Deallocate(pointer);
}

SmallObjectAllocator* g_SmallObjectAllocator = nullptr;

bool InitializeGlobalSmallObjectAllocator()
{
	if (g_SmallObjectAllocator != nullptr)
	{
		return false;
	}

	g_SmallObjectAllocator = new SmallObjectAllocator(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE);

	return true;
}

bool InitializeGlobalSmallObjectAllocator(const int chunkSize, const int maxSmallObjectSize)
{
	if (g_SmallObjectAllocator != nullptr)
	{
		return false;
	}

	g_SmallObjectAllocator = new SmallObjectAllocator(chunkSize, maxSmallObjectSize);

	return true;
}

void DeleteGlobalSmallObjectAllocator()
{
	delete g_SmallObjectAllocator;
}