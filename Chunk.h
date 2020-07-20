#ifndef CHUNK_H_GUARD
#define CHUNK_H_GUARD

#include <iostream>


struct Chunk
{
	void Initialize(std::size_t blockSize, unsigned char blocks);
	void Release();
	void Reset(std::size_t blockSize, unsigned char blocks);
	void* Allocate(std::size_t blockSize);
	void Deallocate(void* pointer, std::size_t blockSize);
	unsigned char* m_PointerToData;
	unsigned char
		m_FirstAvailableBlock,
		m_BlockAvailable;
};

#endif

