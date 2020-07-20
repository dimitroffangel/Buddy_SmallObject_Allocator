#include "Chunk.h"

#include <assert.h>

const size_t DEFAULT_CHUNK_SIZE = 4096;

const size_t UNSIGNED_CHAR_MAX = UCHAR_MAX;

const unsigned DefaultNumberOfBlocks = 8;


void Chunk::Initialize(std::size_t blockSize, unsigned char numberOfBlocks)
{
	m_PointerToData = new unsigned char[blockSize * numberOfBlocks];
	m_FirstAvailableBlock = 0;
	m_BlockAvailable = numberOfBlocks;
	unsigned char i = 0;
	unsigned char* tempPointer = m_PointerToData;

	for (; i != numberOfBlocks; tempPointer += blockSize)
	{
		*tempPointer = ++i;
	}
}


void Chunk::Release()
{
	delete[] m_PointerToData;
}

void Chunk::Reset(std::size_t blockSize, unsigned char numberOfBlocks)
{
	assert(blockSize > 0);
	assert(numberOfBlocks > 0);

	assert((blockSize * numberOfBlocks) / blockSize == numberOfBlocks);

	m_FirstAvailableBlock = 0;
	m_BlockAvailable = numberOfBlocks;

	unsigned char i = 0;

	unsigned char* pointer = m_PointerToData;

	for (; i != numberOfBlocks; pointer += blockSize)
	{
		*pointer = ++i;
	}
}

void* Chunk::Allocate(std::size_t blockSize)
{
	if (!m_BlockAvailable)
		return nullptr;

	unsigned char* pointerResult =
		m_PointerToData + (m_FirstAvailableBlock * blockSize);

	m_FirstAvailableBlock = *pointerResult;
	--m_BlockAvailable;

	return pointerResult;
}

void Chunk::Deallocate(void* deallocatePointer, std::size_t blockSize)
{
	assert(deallocatePointer >= m_PointerToData);

	unsigned char* toReleasePointer = static_cast<unsigned char*>(deallocatePointer);

	assert((toReleasePointer - m_PointerToData) % blockSize == 0);

	*toReleasePointer = m_FirstAvailableBlock;
	m_FirstAvailableBlock = static_cast<unsigned char>((toReleasePointer - m_PointerToData) / blockSize);

	// Truncation check
	assert(m_FirstAvailableBlock == (toReleasePointer - m_PointerToData) / blockSize);
	++m_BlockAvailable;
}