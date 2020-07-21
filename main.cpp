#include <iostream>
#include <vector>

#include "SmallObject.h"

class Foo
{
public:
	int a = 42;
	int b = 32;
	std::string test = "42";

	Foo()
	{
		
	}
};

void Test(Foo* foo1, Foo* foo2)
{
	std::cout << (foo1 < foo2);
}

int main()
{
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

	return 0;
}