#include "BuddyAllocator.h"

#include <iostream>
#include <assert.h>

void BuddyAllocator::SimulateAllocationForLeaves_ForFreeList(size_t numberOfAllocationsOnLeafsNeeded)
{
	int level = MAX_LEVELS - FastLogarithm::Log2_64(LEAF_SIZE);

	while (numberOfAllocationsOnLeafsNeeded > 0 && level >= 0)
	{
		for (size_t i = 0; i < numberOfAllocationsOnLeafsNeeded; i += 2)
		{
			size_t levleSize = GetSizeOfLevel(level);
			size_t uniqueIndex = GetUniqueIndex(m_PointerToData + (i * GetSizeOfLevel(level)), level);
			size_t parentIndex = GetParent(uniqueIndex);

			SetBitToOne_SplitTable(parentIndex);
			//m_SplitTable[parentIndex] = 1;

			if (i % 2 == 0 && i + 1 < numberOfAllocationsOnLeafsNeeded)
			{
				// the buddy of the leaf is taken as well
				SetBitToZero_FreeTable(parentIndex);
				//m_FreeTable[parentIndex] = 0;
			}

			else
			{
				// the buddy is free
				SetBitToOne_FreeTable(parentIndex);
				//m_FreeTable[parentIndex] = 1;
			}
		}

		if (numberOfAllocationsOnLeafsNeeded % 2 != 0 && level != 0)
		{
			// there is a freeSlot there
			*(PtrInt*)(m_PointerToData + sizeof(PtrInt) * level) =
				(PtrInt)(m_PointerToData + (GetSizeOfLevel(level) * numberOfAllocationsOnLeafsNeeded));

			PtrInt* fooProdigy = (PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level));
			FreeListInformation* fooProdigyInfo = (FreeListInformation*)(*fooProdigy);

			*((FreeListInformation*)(m_PointerToData + (GetSizeOfLevel(level) * numberOfAllocationsOnLeafsNeeded))) = { nullptr, nullptr };


			int a = 42;
 		}

		numberOfAllocationsOnLeafsNeeded = numberOfAllocationsOnLeafsNeeded / 2 + !(numberOfAllocationsOnLeafsNeeded % 2 == 0);
			
		--level;
	}
}

void BuddyAllocator::SetBitToOne_FreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	PtrInt tref = (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
	
	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) |= (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res = GetBitFromFreeTable(parentIndex);
}

void BuddyAllocator::SetBitToZero_FreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) &= ~(PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res = GetBitFromFreeTable(parentIndex);
}

inline bool BuddyAllocator::GetBitFromFreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	bool res = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	return (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
}

inline void BuddyAllocator::XOR_Bit_FreeTable(const size_t parentIndex, const bool XOR_Argument)
{
	bool result = GetBitFromFreeTable(parentIndex) ^ XOR_Argument;
	
	if (result == 0)
	{
		SetBitToZero_FreeTable(parentIndex);
	}

	else
	{
		SetBitToOne_FreeTable(parentIndex);
	}

	bool res = GetBitFromFreeTable(parentIndex);
}

void BuddyAllocator::SetBitToOne_SplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) |= (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res = GetBitFromSplitTable(parentIndex);
}

void BuddyAllocator::SetBitToZero_SplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) &= ~(PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	bool res = GetBitFromSplitTable(parentIndex);
}

inline bool BuddyAllocator::GetBitFromSplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	bool res = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	return (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
}

inline void BuddyAllocator::XOR_Bit_SplitTable(const size_t parentIndex, const bool XOR_Argument)
{
	bool result = GetBitFromSplitTable(parentIndex) ^ XOR_Argument;

	if (result == 0)
	{
		SetBitToZero_SplitTable(parentIndex);
	}

	else
	{
		SetBitToOne_SplitTable(parentIndex);
	}

	bool res = GetBitFromSplitTable(parentIndex);
}

void BuddyAllocator::Initialize()
{
	m_PointerToData = new unsigned char[DEFAULT_BUDDY_ALLOCATOR_SIZE];

	//m_FreeLists[0] = m_PointerToData;
	//*static_cast<FreeListInformation*>(m_FreeLists[0]) = { nullptr, nullptr };

	//for (size_t i = 1; i < MAX_LEVELS; ++i)
	//{
	//	m_FreeLists[i] = nullptr;
	//}
	//{
	//	void* startOfTheListInformationPointers = Allocate(LEAF_SIZE);

	//	if (startOfTheListInformationPointers == nullptr)
	//	{
	//		std::cerr << "BuddyAllocator::Initialize startOfTheListInformationPointers == nullptr" << '\n';
	//		return;
	//	}

	//	*static_cast<PtrInt*>(startOfTheListInformationPointers) = (PtrInt)(nullptr);
	//}
	
	{
		*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);

		FreeListInformation* foo = (FreeListInformation*)(m_PointerToData);
		*foo = { nullptr, nullptr };
	}

	size_t size = sizeof(PtrInt);

	for (size_t i = 1; i < MAX_LEVELS; ++i)
	{
		*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * i)) = 0;

		size += sizeof(PtrInt);
	}

	for (size_t i = 0; i < NUMBER_OF_BITSET_FOR_FREE_TABLE; ++i)
	{
		SetBitToZero_FreeTable(i);
		SetBitToZero_SplitTable(i);
	}

	size = size + (2 * NUMBER_OF_BITSET_FOR_FREE_TABLE / 8);

	size_t numberOfAllocationsOnLeafsNeeded = 
		(size) / LEAF_SIZE + !(size % LEAF_SIZE == 0);

	SimulateAllocationForLeaves_ForFreeList(numberOfAllocationsOnLeafsNeeded);
	
	int test = 42;
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
		*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);
		//*(FreeListInformation*)(m_PointerToData) = { nullptr, nullptr };

		//m_FreeLists[0] = m_PointerToData;
		return;
	}

	size_t uniqueIndexOfThePointer = GetUniqueIndex(pointerToFree, levelIndex);
	size_t buddyIndexOfThePointerInLevel = GetBuddy(pointerToFree, levelIndex);
	size_t buddyIndexOfThePointer = GetUniqueIndex(buddyIndexOfThePointerInLevel, levelIndex);
	size_t getParentIndex = GetParent(uniqueIndexOfThePointer);
	size_t parentLevel = levelIndex - 1;
	// the pointer may be to a location, which is split, not used

	
	// check if the level is right
	if (levelIndex >= MAX_LEVELS)
	{
		std::cerr << "BuddyAllocator::Free(void* pointerToFree, size_t levelIndex) levelIndex >= MAX_LEVELS" << '\n';
		return;
	}

	// check if the level points 
	if (uniqueIndexOfThePointer < NUMBER_OF_BITSET_FOR_FREE_TABLE && GetBitFromSplitTable(uniqueIndexOfThePointer) == 1)
	{
		std::cerr << "BuddyAllocator::Free(void* pointerToFree, size_t levelIndex) m_SplitTable[uniqueIndexOfThePointer] = 1" << '\n';
		return;
	}

	while (true)
	{
		// the other buddy is free
		if (GetBitFromFreeTable(getParentIndex) == 1)
		{
			// change the m_FreeTable
			SetBitToZero_FreeTable(getParentIndex);
 			//m_FreeTable[getParentIndex] = 0;

			// add the free slot to the parentLevel of freeSlots

			// declare the two pointers
			// set the slot of the released pointer to 0
			PtrInt* releasedPointer = static_cast<PtrInt*>(pointerToFree);
			*releasedPointer = (PtrInt)(nullptr);

			// -----reorganize the chaining on the level of the releasedPointer
			// get the buddy pointer that is about to be released
			void* buddyPointer = (m_PointerToData + (GetSizeOfLevel(levelIndex) * buddyIndexOfThePointerInLevel));
			
			// set the freeList on that level with value in the pointer
			if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) == (PtrInt)(buddyPointer))
			{
				FreeListInformation* freeSlot = static_cast<FreeListInformation*>(buddyPointer);

				*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) = (PtrInt)(freeSlot->next);

				//m_FreeLists[levelIndex] = static_cast<FreeListInformation*>(buddyPointer)->next;
			}

			else
			{
				((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->previous))->next
					= static_cast<FreeListInformation*>(buddyPointer)->next;

				((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->next))->previous
					= static_cast<FreeListInformation*>(buddyPointer)->previous;
			}

			levelIndex = levelIndex - 1;

			// set the splitTable value of the parent block to 0
			{
				assert(GetBitFromSplitTable(getParentIndex) == 1);
				SetBitToZero_SplitTable(getParentIndex);

				//assert(m_SplitTable[getParentIndex] == 1);
				//m_SplitTable[getParentIndex] = 0;
			}

			// if has reached top, abort the releasing of blocks
			if (levelIndex == 0)
			{
				*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);

				//*(FreeListInformation*)(m_PointerToData) = { nullptr, nullptr };
				//m_FreeLists[0] = m_PointerToData;
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
			SetBitToOne_FreeTable(getParentIndex);
			//m_FreeTable[getParentIndex] = 1;

			// set the head of the linkedList to the pointerToFree location
			if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) == 0)
			{
				//m_FreeLists[levelIndex] = pointerToFree;

				static_cast<FreeListInformation*>(pointerToFree)->next = nullptr;
				static_cast<FreeListInformation*>(pointerToFree)->previous = nullptr;
			}

			else
			{
				((FreeListInformation*)(*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex)))))->previous =
					static_cast<PtrInt*>(pointerToFree);

				static_cast<FreeListInformation*>(pointerToFree)->next = (PtrInt*)(*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * levelIndex)));
				static_cast<FreeListInformation*>(pointerToFree)->previous = nullptr;
			}

			*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * levelIndex)) = (PtrInt)(pointerToFree);
			//m_FreeLists[levelIndex] = pointerToFree;

			// the other buddy is not free, so there cannot be going forward
			return;
		}
	}
}

void BuddyAllocator::Free(void* pointer)
{
	//// assert that the pointer is in the range

	//if (pointer == nullptr)
	//{
	//	return;
	//}

	//size_t levelIndex = 0;
	//// find the pointer where is positioned
	//while (true)
	//{
	//	if (levelIndex == MAX_LEVELS)
	//	{
	//		break;
	//	}

	//	if (m_FreeLists[levelIndex] == nullptr)
	//	{
	//		++levelIndex;
	//	}


	//}
}

void* BuddyAllocator::Allocate(size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		std::cout << "Size is less than the minimum..." << '\n';
		return nullptr;
	}

	// TODO:: assert some stuff...

	size_t initialLevel = m_NumberOfLevels - FastLogarithm::Log2_64(blockSize) + !FastOperationsWithTwo::IsPowerOfTwo(blockSize);

	size_t levelSize = GetSizeOfLevel(initialLevel);
	int level = initialLevel;

	// find a free block while there is a free block
	while (true)
	{
		// if there is a free block in the current level, split it on two or if it is the initial level end the inspection
		if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level))) != 0)
		{
			// get the raw free location
			void* rawFreeSlot = (void*)(*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level))));

			FreeListInformation* freeSlot = static_cast<FreeListInformation*>(rawFreeSlot);

			// change the m_FreeTable on the parent
			{
				size_t uniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

				if (uniqueIndexOfTheFreeSlot != 0)
				{
					size_t indexOfParent = GetParent(uniqueIndexOfTheFreeSlot);

					XOR_Bit_FreeTable(indexOfParent, 1);
					//m_FreeTable[indexOfParent] = 1 ^ m_FreeTable[indexOfParent];
				}
			}

			// set the level pointer to the next free location
			{
				PtrInt* levelPointer = (PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level));
				*(levelPointer) = (PtrInt)(freeSlot->next);
				PtrInt res = PtrInt(freeSlot->next);
			}

			// has acquired a free block on the expected spot, return the free location
			if (level == initialLevel)
			{
				return rawFreeSlot;
			}

			//  has found a way to partition, start going down again
			else
			{
				// change the split table for the newly divided block
				{
					size_t uniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

					assert(GetBitFromSplitTable(uniqueIndexOfTheFreeSlot) == 0);
					SetBitToOne_SplitTable(uniqueIndexOfTheFreeSlot);

					//assert(m_SplitTable[uniqueIndexOfTheFreeSlot] == 0);
					//m_SplitTable[uniqueIndexOfTheFreeSlot] = 1;
				}

				void* freeSlotNext = (static_cast<unsigned char*>(rawFreeSlot) + GetSizeOfLevel(level + 1));

				// assert the newly taken location has odd uniqueIndex
				{
					size_t nextSlotFreeIndex = GetUniqueIndex(freeSlotNext, level + 1);
					assert(nextSlotFreeIndex % 2 == 0);
				}

				// add the new free location to the level that is bellow the current
				*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * (level + 1))) = (PtrInt)(rawFreeSlot);
				
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