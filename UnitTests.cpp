#include "UnitTests.h"

#include <chrono>

#include "SmallObjectAllocator.h"


void UnitTests::Allocate_Via_Buddy_SmallObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);
		//EpicFoo* result1 = new EpicFoo();
		//foos.push_back(result1);
	}


	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];

		foos[i]->~Foo();

		buddyAllocator.operator delete(foos[size - i - 1], sizeof(Foo));

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_SmallObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject&)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();

		buddyAllocator.operator delete(foos[size - i - 1], sizeof(EpicFoo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_AllObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator,const SmallObject& smallObject)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();
		buddyAllocator.operator delete(foos[i]);

		void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		giantFoos[i]->~GiantFoo();
		buddyAllocator.operator delete(giantFoos[i]);


		void* rawPointer3 = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer2) EpicFoo();

		epicFoos.push_back(res3);

		epicFoos[i]->~EpicFoo();
		buddyAllocator.operator delete(epicFoos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_BigObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);
		//EpicFoo* result1 = new EpicFoo();
		//foos.push_back(result1);
	}


	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];

		foos[i]->~GiantFoo();

		buddyAllocator.operator delete(foos[size - i - 1], sizeof(EpicFoo));

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_BigObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);

		foos[i]->~GiantFoo();
		buddyAllocator.operator delete(foos[size - i - 1], sizeof(GiantFoo));
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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

		
		void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		void* rawPointer3 = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer2) EpicFoo();

		epicFoos.push_back(res3);
	}

	for (size_t i = 0; i < size; ++i)
	{
		foos[i]->~Foo();
		buddyAllocator.operator delete(foos[i]);

		epicFoos[i]->~EpicFoo();
		buddyAllocator.operator delete(epicFoos[i]);

		giantFoos[i]->~GiantFoo();
		buddyAllocator.operator delete(giantFoos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_SmallObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		void* rawPointer = smallObject.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);
	}


	for (size_t i = 0; i < size; i++)
	{
		foos[i]->~Foo();

		smallObject.operator delete(foos[size - i - 1], sizeof(Foo));

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_SmallObjects()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		Foo* result1 = new Foo();
		foos.push_back(result1);
	}


	for (size_t i = 0; i < size; i++)
	{
		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_SmallObjects_Add_Delete()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		Foo* result = new Foo();

		foos.push_back(result);

		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_AllObjects_Add_Delete()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		
		Foo* result = new Foo();

		foos.push_back(result);

		delete foos[i];
		
		GiantFoo* res2 = new GiantFoo();

		giantFoos.push_back(res2);

		delete giantFoos[i];


		EpicFoo* res3 = new EpicFoo();

		epicFoos.push_back(res3);

		delete epicFoos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_BigObjects()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		GiantFoo* result1 = new GiantFoo();
		foos.push_back(result1);
	}


	for (size_t i = 0; i < size; i++)
	{
		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_BigObjects_Add_Delete()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		GiantFoo* result1 = new GiantFoo();
		foos.push_back(result1);
	
		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_RandomObject_DeleteRandomPosition()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
		Foo* result = new Foo();

		foos.push_back(result);

		GiantFoo* res2 = new GiantFoo();

		giantFoos.push_back(res2);

		EpicFoo* res3 = new EpicFoo();

		epicFoos.push_back(res3);
	}

	for (size_t i = 0; i < size; ++i)
	{
		delete foos[i];
		
		delete epicFoos[i];
		
		delete giantFoos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}