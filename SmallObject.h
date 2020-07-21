#ifndef SMALLOBJECT_H_GUARD
#define SMALLOBJECT_H_GUARD

#include <mutex>

#include "SmallObjectAllocator.h"

class SmallObject
{
public:
	static void* operator new(size_t size);
	static void operator delete(void* pointer, size_t size);

	SmallObject()
	{
		InitializeGlobalSmallObjectAllocator();
	}

	SmallObject(const int chunkSize, const int maxSmallObjectSize)
	{
		InitializeGlobalSmallObjectAllocator(chunkSize, maxSmallObjectSize);
	}

	virtual ~SmallObject()
	{

	}

private:
	static std::mutex m_LockOperation;
};

#endif