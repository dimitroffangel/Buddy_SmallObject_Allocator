#include <iostream>
#include <vector>

#include "SmallObject.h"
#include "BuddyAllocator.h"
#include "BuddyAllocatorObject.h"
#include "UnitTests.h"

int main()
{	
	SmallObjectAllocator::Initialize();
	BuddyAllocator::Initialize();

	SmallObject smallObject;
	BuddyAllocatorObject buddyAllocator;




	SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}