#include "UnitTests.h"

#include "SmallObjectAllocator.h"

void UnitTests::Allocate_Via_Buddy_SmallObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);
		//EpicFoo* result1 = new EpicFoo();
		//foos.push_back(result1);
	}


	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];

		foos[i]->~Foo();

		buddyAllocator.operator delete(foos[size - i - 1], sizeof(EpicFoo));

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}
}

void UnitTests::Allocate_Via_Buddy_SmallObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator,const SmallObject& smallObject)
{
	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();
		buddyAllocator.operator delete(foos[i]);

		void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		giantFoos[i]->~GiantFoo();
		buddyAllocator.operator delete(foos[i]);


		void* rawPointer3 = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer2) EpicFoo();

		epicFoos.push_back(res3);

		epicFoos[i]->~EpicFoo();
		buddyAllocator.operator delete(foos[i]);
	}
}
