#ifndef BUDDYALLOCATOR_H_GUARD
#define BUDDYALLOCATOR_H_GUARD

#include "FastLogarithm.h"

/*
	totalSize = (1 << numberOfLevels) * leafSize
	sizeOfEachBlockOnLevel(n) = totalSize / (1 << n)
	maxBlocksOfLevel(n) = (1 << n)
	(1 << level) + indexInLevel - 1 -> unqueue index in the buddyAllocator


	(1 << numberOfLevels) - 1 - number of indices
*/

const uint64_t DEFAULT_BUDDY_ALLOCATOR_SIZE = 1 << 16;
const uint64_t LEAF_SIZE = 1 << 7;

struct BuddyAllocator
{
	static const int MAX_LEVELS = 25 ;

public:
	void Initialize();

	//void Free(void* pointer, size_t sizeOfBlock);
	void Free(void* pointer);
	void* Allocate(size_t blockSize);

private:
	PtrInt* m_FreeLists[MAX_LEVELS];
	PtrInt* m_PointerToData;
	size_t m_NumberOfLevels;
	
	inline size_t GetTotalSize() 
	{
		return (1 << m_NumberOfLevels) * LEAF_SIZE;
	}

	inline size_t GetSizeOfLevel(size_t n)
	{
		return GetTotalSize() / (1 << n);
	}

	inline size_t GetLevel(size_t uniqueIndex)
	{
		if (uniqueIndex == 0)
		{
			return 0;
		}

		return FastLogarithm::log2_64(uniqueIndex + 1);
	}

	inline size_t IndexInLevel(size_t uniqueIndex)
	{
		size_t levelOfIndex = GetLevel(uniqueIndex);
		size_t firstUniqueueIndexOnLevel = (1 << levelOfIndex) - 1;

		return uniqueIndex - firstUniqueueIndexOnLevel;
	}

	inline size_t IndexInLevelOf(void* pointer, size_t levelIndex)
	{
		PtrInt* getUnsignedCharPointer = static_cast<PtrInt*>(pointer);

		return (getUnsignedCharPointer - m_PointerToData) / GetSizeOfLevel(levelIndex);
	}

	inline size_t GetBuddy(void* pointer, size_t levelIndex)
	{
		size_t indexInLevelOfPointer = IndexInLevelOf(pointer, levelIndex);

		if (indexInLevelOfPointer % 2 == 0)
		{
			return indexInLevelOfPointer + 1;
		}

		return indexInLevelOfPointer - 1;
	}
}; 


#endif
