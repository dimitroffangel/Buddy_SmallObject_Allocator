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

	//unitTests.Allocate_Via_Buddy_BigObjects(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_BigObjects_Add_Delete(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_AllObjects_Add_Delete(buddyAllocator, smallObject);

	unitTests.Allocate_Via_Slab_SmallObjects(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_SmallObjects_Add_Delete(buddyAllocator, smallObject);
	
	//unitTests.Allocate_Via_Default_BigObjects();
	//unitTests.Allocate_Via_Default_BigObjects_Add_Delete();
	//unitTests.Allocate_Via_Default_AllObjects_Add_Delete();
	unitTests.Allocate_Via_Default_SmallObjects();
	unitTests.Allocate_Via_Default_SmallObjects_Add_Delete();

	SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}