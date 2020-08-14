#include <iostream>
#include <vector>
#include <functional>

#include "SmallObject.h"
#include "BuddyAllocator.h"
#include "BuddyAllocatorObject.h"

class Foo
{
public:
	int a = 42;
	int b = 32;

	int TryStuff(int c, int d, int f) const
	{
		return a + (c+d);
	}

	//~Foo()
	//{
	//	std::cout << "Losing Foo" << '\n';
	//}
};

class EpicFoo
{
	PtrInt first;
	PtrInt second;
	PtrInt third;
	PtrInt fourth;
	PtrInt fifth;
	PtrInt sixth;
	PtrInt seventh;
	PtrInt eighth;
	PtrInt eighth1;
	PtrInt eighth2;
	PtrInt eighth3;
	PtrInt eighth4;
	PtrInt eighth5;
	PtrInt eighth6;
	PtrInt eighth7;
	PtrInt eighth8;
};

void InitializeSystems()
{
	SmallObjectAllocator::Initialize();
}

void ShutdownSystems()
{
	SmallObjectAllocator::Shutdown();
}

int main()
{	
	//SmallObjectAllocator::Initialize();
	BuddyAllocator::Initialize();

	const int blocks = 4;
	int sizeOfFoo = sizeof(Foo);
	int sizeOfEpicFoo = sizeof(EpicFoo);

	//SmallObject smallObject;
	BuddyAllocatorObject buddyAllocator;


	const int size = 1000000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo)
		void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();
		
		foos.push_back(result);
		//EpicFoo* result1 = new EpicFoo();
		//foos.push_back(result1);
	}

	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];
		
		foos[i]->~EpicFoo();

		buddyAllocator.operator delete(foos[i], sizeof(EpicFoo));

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);
		

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	//SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}