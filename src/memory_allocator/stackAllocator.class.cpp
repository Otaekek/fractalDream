
#include "stackAllocator.class.hpp"
#include <cstdio>

stackAllocator::stackAllocator()
{
	size = DEFAULT_ALLOC_SIZE;
	index = 0;
	data = new char[DEFAULT_ALLOC_SIZE];
}

stackAllocator::stackAllocator(uint32_t _size)
{
	size = _size;
	index = 0;
	data = new char[_size];
}

stackAllocator::~stackAllocator()
{
	delete [] data;
}

void stackAllocator::init(uint32_t _size)
{
	data = new char[_size];
}

void stackAllocator::all_mem_free()
{
	size = 0;
	index = 0;
}

bool stackAllocator::can_alloc(uint32_t _size)
{
	return (_size + index < size);
}

void *stackAllocator::mem_alloc(uint32_t _size)
{
	index += _size;
	return (&data[index - _size]);
}

void stackAllocator::mem_free(uint32_t _size)
{
	index -= _size;
}

void *stackAllocator::get_data_pointer()
{
	return (data);
}

void *stackAllocator::get_offset()
{
	return (&data[index]);
}

uint32_t	stackAllocator::get_size()
{
	return (index);
}