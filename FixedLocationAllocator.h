#include <climits>

#ifndef FIXEDALLOCATOR_H_GUARD
#define FIXEDALLOCATOR_H_GUARD

const size_t DEFAULT_CHUNK_SIZE = 4096;

const size_t UNSIGNED_CHAR_MAX = UCHAR_MAX;

const unsigned DefaultNumberOfBlocks = 8;

class FixedLocationAllocator
{
public:
	FixedLocationAllocator(void*, const unsigned char, const size_t, const size_t);
	~FixedLocationAllocator();

public:
	void* Allocate();
	void Deallocate(void* pointer);

private:
	unsigned char* m_PointerToData;
	//Chunk* m_RecentlyAllocatedChunk;
	//Chunk* m_RecentlyDeallocatedChunk;

	inline void InitializeChunk(void* chunkPointerToData, const size_t blockSize, const size_t numberOfBlockss);
	inline void ChunkAllocation(const size_t blockSize, const size_t chunkIndex);
};


#endif