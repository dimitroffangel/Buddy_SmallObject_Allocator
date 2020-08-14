#include <iostream>
#include <vector>

#include "SmallObject.h"
#include "UnitTests.h"

int main()
{	
	SmallObjectAllocator::Initialize();
	BuddyAllocator::Initialize();

	SmallObject smallObject;
	BuddyAllocatorObject buddyAllocator;

	UnitTests unitTests;

	unitTests.Allocate_Via_Buddy_SmallObjects(buddyAllocator, smallObject);

	unitTests.Allocate_Via_Default_SmallObjects();

	SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}