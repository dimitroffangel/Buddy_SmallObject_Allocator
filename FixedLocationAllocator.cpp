#include "FixedLocationAllocator.h"

#include <assert.h>
#include <iostream>

#include "FastLogarithm.h"

FixedLocationAllocator::FixedLocationAllocator(void* location, const unsigned char blockSize, 
	const size_t sizeOfLocation, const unsigned char numberOfChunksToAllocate)
{
	assert(location != nullptr);

	m_PointerToData = static_cast<unsigned char*>(location);

	const size_t preliminarySize = 
		NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + 2 * sizeof(PtrInt) + numberOfChunksToAllocate * (2 * sizeof(unsigned char));
	
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

	*m_PointerToData = blockSize;
	*(m_PointerToData + 1) = static_cast<unsigned char>(numberOfBlocks);
	*(m_PointerToData + 2) = numberOfChunksToAllocate;
	*(m_PointerToData + 3) = 0; // byte used only for alignment

	// allocate the pointers
	for (size_t i = 0; i < NUMBER_OF_POINTERS; ++i)
	{
		*(PtrInt*)(m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + i * sizeof(PtrInt)) = 0;
	}

	// allocate preamble information for the chunks
	for (size_t i = 0; i < SIZE_OF_CHUNK_INFO_NEEDED; ++i)
	{
		unsigned char* currentChunkInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
			NUMBER_OF_POINTERS * sizeof(PtrInt) + i * SIZE_OF_CHUNK_INFO_NEEDED;

		// *currentChunkInfo -> firstAvailableBlock
		*static_cast<unsigned char*>(currentChunkInfo) = 0;
		
		// *(currentChunkInfo + 1) -> m_BlocksAvailable
		*static_cast<unsigned char*>(currentChunkInfo + 1) = numberOfBlocks;
	}

	// initialize the chunk
	for (size_t i = 0; i < numberOfChunksToAllocate; ++i)
	{
		void* currentChunk = m_PointerToData + preliminarySize + i *(numberOfBlocks * blockSize);
		InitializeChunk(currentChunk, numberOfBlocks);
	}
}

void* FixedLocationAllocator::Allocate()
{
	return nullptr;
}

void FixedLocationAllocator::InitializeChunk(void* chunkPointerToData, const size_t numberOfBlocks)
{
	assert(chunkPointerToData != nullptr);

	const size_t blockSize = *m_PointerToData;

	unsigned char i = 0;
	unsigned char* tempPointer = static_cast<unsigned char*>(chunkPointerToData);

	for (; i != numberOfBlocks; tempPointer += blockSize)
	{
		*tempPointer = ++i;
	}
}

inline void* FixedLocationAllocator::ChunkAllocation(const size_t chunkIndex)
{	
	const size_t blockSize = *m_PointerToData;

	unsigned char* chunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + chunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

	unsigned char& firstAvailableBlock = *chunkPreambleInfo;

	unsigned char& blockAvailable = *(chunkPreambleInfo + 1);

	unsigned char* chunkPointer;
	
	const unsigned char numberOfChunks = *(m_PointerToData + 2);
	{
		chunkPointer = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
			NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks + chunkIndex;
	}

	if (blockAvailable == 0)
	{
		std::cout << "Chunk is empty cannot allocate here..." << '\n';
		return nullptr;
	}

	unsigned char* pointerResult =
		chunkPointer + (firstAvailableBlock * blockSize);

	firstAvailableBlock = *pointerResult;
	--blockAvailable;

	return pointerResult;
}

inline void FixedLocationAllocator::ChunkDeallocation(void* pointerToFree, const size_t chunkIndex)
{
	if (pointerToFree == nullptr)
	{
		std::cerr << "FixedLocationAllocator::ChunkDeallocation void* pointerToFree is nullptr" << '\n';
		return;
	}

	const size_t blockSize = *m_PointerToData;

	unsigned char* chunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + chunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

	unsigned char& firstAvailableBlock = *chunkPreambleInfo;

	unsigned char& blockAvailable = *(chunkPreambleInfo + 1);

	if (!((uintptr_t)(pointerToFree) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+(uintptr_t)(blockSize * (*(m_PointerToData + 1)))))
	{
		std::cerr << "FixedLocationAllocator::ChunkDeallocation Deallocate pointer is off the range of the Chunk" << '\n';
		return;
	}

	unsigned char* toReleasePointer = static_cast<unsigned char*>(pointerToFree);

	// alignment check
	assert((toReleasePointer - m_PointerToData) % blockSize == 0);

	*toReleasePointer = firstAvailableBlock;
	firstAvailableBlock = static_cast<unsigned char>((toReleasePointer - m_PointerToData) / blockSize);

	// Truncation check
	assert(firstAvailableBlock == (toReleasePointer - m_PointerToData) / blockSize);
	++blockAvailable;
}
