#include <iostream>
#include <vector>
#include <functional>

#include "SmallObject.h"

class Foo
{
public:
	int a = 42;
	int b = 32;
	std::string test = "42";


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

	//*fooPtr = Foo();
	//fooPtr->b = 5;

	//chunk.Deallocate(fooPtr, sizeof(Foo));

	//Foo* currentPtr = static_cast<Foo*>(chunk.Allocate(sizeOfFoo));
	//*currentPtr = Foo();


	SmallObject smallObject;
	void* result = smallObject.operator new(sizeOfFoo);


	ShutdownSystems();

	return 0;
}