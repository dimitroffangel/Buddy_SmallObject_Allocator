#include "BuddyAllocator.h"

#include <iostream>
#include <assert.h>

void BuddyAllocator::Initialize()
{
	m_PointerToData = new unsigned char[DEFAULT_BUDDY_ALLOCATOR_SIZE];

	m_FreeLists[0] = m_PointerToData;
	*static_cast<FreeListInformation*>(m_FreeLists[0]) = { nullptr, nullptr };

	for (size_t i = 1; i < MAX_LEVELS; ++i)
	{
		m_FreeLists[i] = nullptr;
	}
}

void BuddyAllocator::Free(void* pointerToFree, size_t levelIndex)
{
	// TODO::: assert that the pointer is in the range

	if (pointerToFree == nullptr)
	{
		return;
	}

	// TODO:: that the pointerToFree and level are valid

	if (levelIndex == 0)
	{
		m_FreeLists[0] = m_PointerToData;
		return;
	}

	size_t uniqueIndexOfThePointer = GetUniqueIndex(pointerToFree, levelIndex);
	size_t buddyIndexOfThePointerInLevel = GetBuddy(pointerToFree, levelIndex);
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
			PtrInt* releasedPointer = static_cast<PtrInt*>(pointerToFree);
			*releasedPointer = (PtrInt)(nullptr);

			// -----reorganize the chaining on the level of the releasedPointer
			// get the buddy pointer that is about to be released
			void* buddyPointer = (m_PointerToData + (GetSizeOfLevel(levelIndex) * buddyIndexOfThePointerInLevel));
			
			// set the freeList on that level with value in the pointer
			if (m_FreeLists[levelIndex] == buddyPointer)
			{
				m_FreeLists[levelIndex] = static_cast<FreeListInformation*>(buddyPointer)->next;
			}

			else
			{
				((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->previous))->next
					= static_cast<FreeListInformation*>(buddyPointer)->next;

				((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->next))->previous
					= static_cast<FreeListInformation*>(buddyPointer)->previous;
			}

			// get the parent pointer and do the same stuff
			
			levelIndex = levelIndex - 1;
			// if has reached top, abort the releasing of blocks
			if (levelIndex == 0)
			{
				m_FreeLists[0] = m_PointerToData;
				return;
			}

			// find the one with odd uniqueIndex
			// NOTE:::->   levelIndex > 0 => has a buddy

			if (uniqueIndexOfThePointer % 2 == 0)
			{
				std::swap(pointerToFree, buddyPointer);
			}


			// the parent address starts with the address of the current pointer
			uniqueIndexOfThePointer = GetUniqueIndex(pointerToFree, levelIndex);


			buddyIndexOfThePointerInLevel = GetBuddy(pointerToFree, levelIndex);
			buddyIndexOfThePointer = GetUniqueIndex(buddyIndexOfThePointerInLevel, levelIndex);
			getParentIndex = GetParent(uniqueIndexOfThePointer);
			parentLevel = levelIndex - 1;
		}

		else
		{
			// change the m_FreeTable
			m_FreeTable[getParentIndex] = 1;

			// declare the two pointers
			// set the slot of the released pointer to 0
			PtrInt* releasedPointer = static_cast<PtrInt*>(pointerToFree);
			*releasedPointer = (PtrInt)(nullptr);

			static_cast<FreeListInformation*>(m_FreeLists[levelIndex])->previous = releasedPointer;

			static_cast<FreeListInformation*>(pointerToFree)->next = static_cast<PtrInt*>(m_FreeLists[levelIndex]);
			static_cast<FreeListInformation*>(pointerToFree)->previous = nullptr;

			m_FreeLists[levelIndex] = pointerToFree;
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

	size_t levelIndex = 0;
	// find the pointer where is positioned
	while (true)
	{
		if (levelIndex == MAX_LEVELS)
		{
			break;
		}

		if (m_FreeLists[levelIndex] == nullptr)
		{
			++levelIndex;
		}


	}
}

void* BuddyAllocator::Allocate(size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		std::cout << "Size is less than the minimum..." << '\n';
		return nullptr;
	}

	// TODO:: assert some stuff...

	size_t initialLevel;
	
	if (FastOperationsWithTwo::IsPowerOfTwo(blockSize))
	{
		initialLevel = m_NumberOfLevels - FastLogarithm::log2_64(blockSize);
	}

	// get the minimum above the size
	else
	{
		initialLevel = m_NumberOfLevels - FastLogarithm::log2_64(blockSize) + 1;
	}

	size_t levelSize = GetSizeOfLevel(initialLevel);
	int level = initialLevel;

	// find a free block while there is a free block
	while (true)
	{
		// if there is a free block in the current level, split it on two or if it is the initial level end the inspection
		if (m_FreeLists[level] != nullptr)
		{
			// get the raw free location
			void* rawFreeSlot = m_FreeLists[level];

			FreeListInformation* freeSlot = static_cast<FreeListInformation*>(m_FreeLists[level]);

			// set the level pointer to the next free location
			m_FreeLists[level] = freeSlot->next;

			// change the m_FreeTable on the parent
			{
				size_t uniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

				size_t indexOfParent = GetParent(uniqueIndexOfTheFreeSlot);

				if(uniqueIndexOfTheFreeSlot != 0)
					m_FreeTable[indexOfParent] = 1 ^ m_FreeTable[indexOfParent];
			}

			// has acquired a free block on the expected spot, return the free location
			if (level == initialLevel)
			{
				return rawFreeSlot;
			}

			//  has found a way to partition, start going down again
			else
			{
				void* freeSlotNext = (static_cast<unsigned char*>(rawFreeSlot) + GetSizeOfLevel(level + 1));

				// assert the newly taken location has odd uniqueIndex
				{
					size_t nextSlotFreeIndex = GetUniqueIndex(freeSlotNext, level + 1);
					assert(nextSlotFreeIndex % 2 == 0);
				}

				// add the new free location to the level that is bellow the current
				m_FreeLists[level + 1] = rawFreeSlot;

				// set the previous and next to the next free location
				*static_cast<FreeListInformation*>(rawFreeSlot) = { nullptr, static_cast<PtrInt*>(freeSlotNext) };
				*static_cast<FreeListInformation*>(freeSlotNext) = { static_cast<PtrInt*>(rawFreeSlot), nullptr };

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