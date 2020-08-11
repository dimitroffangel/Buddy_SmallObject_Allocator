#ifndef BUDDYALLOCATOR_H_GUARD
#define BUDDYALLOCATOR_H_GUARD

#include "FastLogarithm.h"
#include <bitset>

/*
	totalSize = (1 << numberOfLevels) * leafSize
	sizeOfEachBlockOnLevel(n) = totalSize / (1 << n)
	maxBlocksOfLevel(n) = (1 << n)
	(1 << level) + indexInLevel - 1 -> unqueue index in the buddyAllocator


	(1 << numberOfLevels) - 1 - number of indices
*/

const uint64_t DEFAULT_BUDDY_ALLOCATOR_SIZE = 1 << 16;
const uint64_t LEAF_SIZE = 1 << 7;

struct FreeListInformation
{
	PtrInt* previous = nullptr;
	PtrInt* next = nullptr;
};

struct BuddyAllocator
{
	static const int MAX_LEVELS = 16;
	static const int NUMBER_OF_BITSET_FOR_FREE_TABLE = DEFAULT_BUDDY_ALLOCATOR_SIZE / LEAF_SIZE;

public:
	void Initialize();

	void Free(void* pointer, size_t levelIndex);
	void Free(void* pointer);
	void* Allocate(size_t blockSize);

private:
	void* m_FreeLists[MAX_LEVELS];
	std::bitset<NUMBER_OF_BITSET_FOR_FREE_TABLE> m_FreeTable;
	size_t m_NumberOfLevels = MAX_LEVELS;

	unsigned char* m_PointerToData;
	
	inline size_t GetTotalSize() 
	{
		//return (1 << m_NumberOfLevels) * LEAF_SIZE;
		return (1 << m_NumberOfLevels);
	}


	// TotalSize / (2^levelIndex) = sizeOfEachBlockThere 
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
		size_t firstUniqueIndexOnLevel = (1 << levelOfIndex) - 1;

		return uniqueIndex - firstUniqueIndexOnLevel;
	}

	inline size_t IndexInLevelOf(void* pointer, size_t levelIndex)
	{
		unsigned char* getUnsignedCharPointer = static_cast<unsigned char*>(pointer);

		return (getUnsignedCharPointer - m_PointerToData) / GetSizeOfLevel(levelIndex);
	}

	inline size_t GetUniqueIndex(void* pointer, size_t levelIndex)
	{
		/*
			indexInLevelOfThePointer = uniqueIndex - firstUniqueIndexOnLevel
			<=>
			uniqueIndex = indexInLevelOfThePointer + firstUniqueIndexOnLevel
		*/ 
		
		size_t firstUniqueIndexOnLevel = (1 << levelIndex) - 1;
		size_t indexInLevelOfThePointer = IndexInLevelOf(pointer, levelIndex);
	
		return indexInLevelOfThePointer + firstUniqueIndexOnLevel;
	}

	inline size_t GetParent(size_t uniqueIndex)
	{
		if (uniqueIndex == 0)
		{
			return 0;
		}

		if (uniqueIndex % 2 == 0)
		{
			return (uniqueIndex - 1) / 2;
		}

		return uniqueIndex / 2;
	}

	inline size_t GetUniqueIndex(size_t localIndexInLevel, size_t levelIndex)
	{
		size_t firstUniqueIndexInLevel = (1 << levelIndex) - 1;
		
		return localIndexInLevel + firstUniqueIndexInLevel;
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
