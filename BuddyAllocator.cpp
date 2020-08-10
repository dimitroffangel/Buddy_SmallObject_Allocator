#include "BuddyAllocator.h"

#include <iostream>
#include <assert.h>

void BuddyAllocator::Initialize()
{
	m_PointerToData = new PtrInt[DEFAULT_BUDDY_ALLOCATOR_SIZE];

	m_FreeLists[0] = m_PointerToData;

	for (size_t i = 1; i < MAX_LEVELS; ++i)
	{
		m_FreeLists[i] = nullptr;
	}
}

void BuddyAllocator::Free(void* pointer, size_t levelIndex)
{
	// assert that the pointer is in the range

	if (pointer == nullptr)
	{
		return;
	}

	size_t uniqueIndexOfThePointer = GetUniqueIndex(pointer, levelIndex);
	size_t buddyIndexOfThePointerInLevel = GetBuddy(pointer, levelIndex);
	size_t buddyIndexOfThePointer = GetUniqueIndex(buddyIndexOfThePointerInLevel, levelIndex);
	size_t getParentIndex = GetParent(uniqueIndexOfThePointer);
	size_t parentLevel = levelIndex - 1;
	// the pointer may be to a location, which is split, not used


	while (true)
	{
		// the other buddy is free
		if (m_FreeTable[getParentIndex] == 1)
		{
			// change the m_FreeTable
			m_FreeTable[getParentIndex] = 0;

			// add the free slot to the parentLevel of freeSlots
			
			// declare the two pointers
			// set the slot of the released pointer to 0
			PtrInt* releasedPointer = static_cast<PtrInt*>(pointer);
			*releasedPointer = (PtrInt)(nullptr);

			// -----change the chaining in the level of the releasedPointer
			// get the buddy pointer that is about to be released
			PtrInt* buddyPointer = m_PointerToData + (GetSizeOfLevel(levelIndex) * buddyIndexOfThePointerInLevel);
			
			// set the freeList on that level with value in the pointer
			if (m_FreeLists[levelIndex] == buddyPointer)
			{
				m_FreeLists[levelIndex] = (PtrInt*)(*buddyPointer);
			}

			else
			{

			}
		}

	}
}

void BuddyAllocator::Free(void* pointer)
{
	// assert that the pointer is in the range

	if (pointer == nullptr)
	{
		return;
	}


}

void* BuddyAllocator::Allocate(size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		return nullptr;
	}

	// assert some stuff...

	size_t initialLevel;
	
	if (FastOperationsWithTwo::IsPowerOfTwo(blockSize))
	{
		initialLevel = FastLogarithm::log2_64(blockSize);
	}
	else
	{
		initialLevel = FastLogarithm::log2_64(blockSize) + 1;
	}

	size_t levelSize = GetSizeOfLevel(initialLevel);
	int level = initialLevel;


	while (true)
	{
		if (m_FreeLists[level] != nullptr)
		{
			PtrInt* freeSlot = (PtrInt*)*m_FreeLists[level];

			m_FreeLists[level] = (PtrInt*)(*m_FreeLists[level]);

			{
				size_t getUniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

				assert(m_FreeTable[getUniqueIndexOfTheFreeSlot] == 0);

				if (m_FreeTable[getUniqueIndexOfTheFreeSlot] != 0)
				{
					std::cerr << "BuddyAllocator::Allocate() m_FreeTable[getUniqueIndexOfTheFreeSlot] != 0" << "\n";
					return nullptr;
				}

				
			}


			if (level == initialLevel)
			{
				return freeSlot;
			}

			//  has found a way to partition start going down again
			else
			{
				m_FreeLists[level + 1] = freeSlot;
				*freeSlot = (PtrInt)(freeSlot + GetSizeOfLevel(level));

				*(freeSlot + GetSizeOfLevel(level)) = (PtrInt)nullptr;


				++level;
			}
		}

		else
		{
			--level;
	
			if (level == -1)
			{
				break;
			}
		}
	}

	std::cout << "BuddyAllocator::Allocate() No Free Slot available..." << '\n';
	return nullptr;
}
