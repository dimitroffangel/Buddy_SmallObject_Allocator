#include "BuddyAllocator.h"

#include <iostream>

void BuddyAllocator::Initialize()
{
	m_PointerToData = new PtrInt[DEFAULT_CHUNK_SIZE];

	m_FreeLists[0] = m_PointerToData;

	for (size_t i = 1; i < MAX_LEVELS; ++i)
	{
		m_FreeLists[i] = nullptr;
	}
}

void BuddyAllocator::Free(void* pointer)
{
}

void* BuddyAllocator::Allocate(size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		return nullptr;
	}

	// assert some stuff...

	size_t initialLevel = FastLogarithm::log2_64(blockSize) + 1;
	size_t levelSize = GetSizeOfLevel(initialLevel);
	int level = initialLevel;


	while (true)
	{
		if (m_FreeLists[level] != nullptr)
		{
			PtrInt* freeSlot = (PtrInt*)*m_FreeLists[level];

			m_FreeLists[level] = (PtrInt*)(*m_FreeLists[level]);

			if (level == initialLevel)
			{
				return freeSlot;
			}

			//  has found a way to partition start going down again
			else
			{
				m_FreeLists[level + 1] = freeSlot;
				*freeSlot = (PtrInt)(freeSlot + GetSizeOfLevel(level));

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
