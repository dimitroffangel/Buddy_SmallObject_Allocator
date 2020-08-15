#include "FixedLocationAllocator.h"

#include <assert.h>

#include "FastLogarithm.h"

FixedLocationAllocator::FixedLocationAllocator(void* location, const unsigned char blockSize, 
	const size_t sizeOfLocation, const size_t numberOfChunksToAllocate)
{
	assert(location != nullptr);

	m_PointerToData = static_cast<unsigned char*>(location);

	const size_t preliminarySize = sizeof(unsigned char) + 2 * sizeof(PtrInt) + numberOfChunksToAllocate * (2 * sizeof(unsigned char));
	
	size_t numberOfBlocks = DEFAULT_CHUNK_SIZE / blockSize;

	// calculate the number of blocks for each chunk
	if (numberOfBlocks > UNSIGNED_CHAR_MAX)
	{
		numberOfBlocks = UNSIGNED_CHAR_MAX;
	}

	else if (numberOfBlocks == 0)
	{
		// TODO:: check this
		numberOfBlocks = DefaultNumberOfBlocks;
	}


	assert(numberOfBlocks == static_cast<unsigned char>(numberOfBlocks));
	assert(preliminarySize + numberOfBlocks * blockSize * numberOfChunksToAllocate <= sizeOfLocation);

	*m_PointerToData = static_cast<unsigned char>(blockSize);


	// allocate the pointers
	const size_t numberOfPointers = 2;
	for (size_t i = 0; i < numberOfBlocks; ++i)
	{
		*(PtrInt*)(m_PointerToData + sizeof(unsigned char) + i * sizeof(PtrInt)) = 0;
	}

	// allocate preamble information for the chunks
	const size_t chunkInfoNeeded = 2 * sizeof(unsigned char);

	for (size_t i = 0; i < numberOfChunksToAllocate; ++i)
	{
		void* currentChunkInfo = m_PointerToData + sizeof(unsigned char) + numberOfPointers * sizeof(PtrInt) + i * chunkInfoNeeded;

		*static_cast<unsigned char*>(currentChunkInfo) = 0;
		*static_cast<unsigned char*>(currentChunkInfo) = numberOfBlocks;
	}

	// initialize the chunk
	for (size_t i = 0; i < numberOfChunksToAllocate; ++i)
	{
		void* currentChunk = m_PointerToData + preliminarySize + i *(numberOfBlocks * blockSize);
		InitializeChunk(currentChunk, numberOfBlocks, blockSize);
	}
}

void* FixedLocationAllocator::Allocate()
{
	return nullptr;
}

void FixedLocationAllocator::InitializeChunk(void* chunkPointerToData, const size_t blockSize, const size_t numberOfBlocks)
{
	assert(chunkPointerToData != nullptr);

	unsigned char i = 0;
	unsigned char* tempPointer = static_cast<unsigned char*>(chunkPointerToData);

	for (; i != numberOfBlocks; tempPointer += blockSize)
	{
		*tempPointer = ++i;
	}
}
