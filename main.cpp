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
	std::cout << FastOperationsWithTwo::IsPowerOfTwo(2);

	PtrInt test = -1;

	//int foo = 42;
	//int* c = &foo;
	//unsigned b = (unsigned)(c);

	return 0;


	SmallObjectAllocator::Initialize();


	const int blocks = 4;
	int sizeOfFoo = sizeof(Foo);

	//Chunk chunk;
	//chunk.Initialize(sizeOfFoo, blocks);

	//Foo* fooPtr = static_cast<Foo*>(chunk.Allocate(sizeOfFoo));

	// *fooPtr = Foo();
	// fooPtr->b = 5;

	//chunk.Deallocate(fooPtr, sizeof(Foo));

	//Foo* currentPtr = static_cast<Foo*>(chunk.Allocate(sizeOfFoo));
	//*currentPtr = Foo();


	SmallObject smallObject;

	const int size = 1000000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeOfFoo);


		Foo* result = new (rawPointer) Foo();
		result->a = i;
		
		foos.push_back(result);

	//	Foo* result1 = new Foo();
	//	foos.push_back(result1);
	}

	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];
		
		foos[i]->~Foo();
		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		smallObject.operator delete(foos[i], sizeOfFoo);
		

		//std::cout << size - i - 1 << " " << foos[size - i - 1]->a << "\n" ;
	}

	SmallObjectAllocator::Shutdown();

	return 0;
}