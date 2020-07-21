#include <iostream>
#include <vector>
#include <functional>

#include "SmallObject.h"

class Foo
{
public:
	int a = 42;
	int b = 32;

	int TryStuff(int c, int d, int f) const
	{
		return a + (c+d);
	}
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
	InitializeSystems();

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
		Foo* result = static_cast<Foo*>(smallObject.operator new(sizeOfFoo));

		*result = Foo();
		foos.push_back(result);

	//	Foo* result1 = new Foo();
	//	foos.push_back(result1);
	}

	for (size_t i = 0; i < size; i++)
	{
		//delete foos[i];

		//smallObject.operator delete(foos[size - i - 1], sizeOfFoo);
		smallObject.operator delete(foos[i], sizeOfFoo);
	}

	ShutdownSystems();	

	return 0;
}