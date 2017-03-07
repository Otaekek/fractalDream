#include "poolAllocator.class.hpp"
#include <stdio.h>
#include <cstdlib>

poolAllocator::poolAllocator()
{
	data = nullptr;
}

poolAllocator::poolAllocator(uint32_t bloc_size, uint32_t bloc_num)
{
	poolAllocator::t_bloc *bloc;
	poolAllocator::t_bloc *last;

	data = new char[(bloc_size + sizeof(t_bloc)) * (bloc_num + 1)];
	free_list = (poolAllocator::t_bloc*)data;
	bloc = free_list;
	for (uint32_t i = 0; i < bloc_num; i++)
	{
		last = bloc;
		bloc->mem = (char*)(bloc) + sizeof(t_bloc);
		bloc->size = bloc_size;
		bloc->next_elem = (t_bloc*)(data + ((bloc_size + sizeof(t_bloc)) * (i + 1)));
		bloc = bloc->next_elem;
	}
	free_list_last = last;
	free_list_last->next_elem = nullptr;
	bloc->next_elem = nullptr;
}

void 		poolAllocator::modifySize(uint32_t bloc_size, uint32_t bloc_num)
{
	poolAllocator::t_bloc *bloc;
	poolAllocator::t_bloc *last;
	if (data)
		delete [] data;
	data = new char[(bloc_size + sizeof(t_bloc)) * (bloc_num + 1)];
	free_list = (poolAllocator::t_bloc*)data;
	bloc = free_list;
	for (uint32_t i = 0; i < bloc_num; i++)
	{
		last = bloc;
		bloc->mem = (char*)(bloc) + sizeof(t_bloc);
		bloc->size = bloc_size;
		bloc->next_elem = (t_bloc*)(data + ((bloc_size + sizeof(t_bloc)) * (i + 1)));
		bloc = bloc->next_elem;
	}
	free_list_last = last;
	free_list_last->next_elem = nullptr;
	bloc->next_elem = nullptr;
}

poolAllocator::~poolAllocator()
{
	delete [] data;
}

void 		*poolAllocator::mem_alloc(void)
{
	t_bloc *ret;

	if (free_list == nullptr)
	{
		return free_list;
	}
	ret = (t_bloc*)free_list->mem;
	free_list = free_list->next_elem;
	return (ret);
}

void 		poolAllocator::mem_free(void *mem)
{
	t_bloc *bloc = (t_bloc*)((char*)mem - sizeof(t_bloc));
	if (free_list == nullptr)
	{
		free_list = bloc;
		free_list_last = bloc;
		bloc->next_elem = nullptr;
	}
	else
	{
		free_list_last->next_elem = bloc;
		free_list_last = bloc;
		bloc->next_elem = nullptr;
	}
}