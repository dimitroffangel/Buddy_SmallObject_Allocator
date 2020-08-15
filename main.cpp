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
	
	std::cout << "Buddy allocator..." << '\n';
	
	//unitTests.Allocate_Via_Buddy_BigObjects(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_BigObjects_Add_Delete(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_AllObjects_Add_Delete(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_SmallObjects(buddyAllocator, smallObject);
	//unitTests.Allocate_Via_Buddy_SmallObjects_Add_Delete(buddyAllocator, smallObject);

	std::cout << "Slab allocator..." << '\n';
	
	unitTests.Allocate_Via_Slab_SmallObjects(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_SmallObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_AllObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_BigObjects(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_BigObjects_Add_Delete(buddyAllocator, smallObject);

	std::cout << "Default allocator..." << '\n';

	unitTests.Allocate_Via_Default_BigObjects();
	unitTests.Allocate_Via_Default_BigObjects_Add_Delete();
	unitTests.Allocate_Via_Default_AllObjects_Add_Delete();
	unitTests.Allocate_Via_Default_SmallObjects();
	unitTests.Allocate_Via_Default_SmallObjects_Add_Delete();
	unitTests.Allocate_Via_Default_RandomObject_DeleteRandomPosition();


	SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}