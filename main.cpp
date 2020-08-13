#include <iostream>
#include <vector>
#include <functional>

#include "SmallObject.h"
#include "BuddyAllocator.h"

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
	{
		PtrInt* a = new PtrInt(42);

		PtrInt* b = new PtrInt;

		*b = (PtrInt)a;

		PtrInt* c = new PtrInt;

		*c = (PtrInt)(&a);

		PtrInt* f = (PtrInt*)(*c);
		PtrInt* h = (PtrInt*)(*b);

		int d = 42;
	}

	{
		PtrInt* foo = new PtrInt(1);
	
		unsigned char* testArr = new unsigned char[16];
		
		for (size_t i = 0; i < 8; ++i)
		{
			testArr[i] = 0;
		}

		PtrInt* test = (PtrInt*)(testArr);
		size_t sze = sizeof(PtrInt);

		bool res1 = *test & (1 << 1);

		*test |= (1 << 1);

		bool res2 = *test & (1 << 1);

		//std::cout << test[0] << " " << test[8] << '\n';

		*foo = (*foo) & ~(1 << 2);

		*foo = (*foo) | (1 << 3);

		int breakFoo = 1 << 5 % 6;
		int breakFoo2 = 1 << (5 % 6);

		int test42 = 42;
	}

	{
		PtrInt* foo = new PtrInt(0);
		PtrInt nonFoo = 1;

		for (size_t i = 0; i < 64; ++i)
		{
			if(i != 63)
				nonFoo *= 2;

			PtrInt res = PtrInt(1) << i;
			*foo |= res;

		}



		for (size_t i = 0; i < 63; ++i)
		{
		}

		std::cout << nonFoo << '\n';
	}


	SmallObjectAllocator::Initialize();

	const int blocks = 4;
	int sizeOfFoo = sizeof(Foo);
	int sizeOfEpicFoo = sizeof(EpicFoo);

	SmallObject smallObject;
	BuddyAllocator buddyAllocator;
	buddyAllocator.Initialize();

	const int size = 1000000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < 510; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo)
		void* rawPointer = buddyAllocator.Allocate(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();
		
		foos.push_back(result);
	//	Foo* result1 = new Foo();
	//	foos.push_back(result1);
	}

	for (size_t i = 0; i < 510; i++)
	{
		//delete foos[i];
		
		foos[i]->~EpicFoo();

		buddyAllocator.Free(foos[i], 9);

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		//smallObject.operator delete(foos[i], sizeOfFoo);
		

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	SmallObjectAllocator::Shutdown();

	return 0;
}