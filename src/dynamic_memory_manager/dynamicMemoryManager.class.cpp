#include "dynamicMemoryManager.class.hpp"

poolAllocator 		dynamicMemoryManager::allocator[NUM_CLUSTERS];
uint32_t			dynamicMemoryManager::referencer = 1;
uint32_t			dynamicMemoryManager::cluster_id = 0;
void 				*dynamicMemoryManager::ref_to_ptr[MAX_REF] = {nullptr};
void 				*dynamicMemoryManager::ref_to_bloc[MAX_REF] = {nullptr};

uint32_t			dynamicMemoryManager::cluster_init(uint32_t sizeBloc, uint32_t numBloc)
{
	cluster_id++;
	allocator[cluster_id].modifySize(sizeBloc, numBloc);
	return (cluster_id);
}

uint32_t			dynamicMemoryManager::create_slot(uint32_t type_id)
{
	void *bloc;

	bloc = allocator[type_id].mem_alloc();
	ref_to_ptr[referencer % MAX_REF] = bloc;
	ref_to_bloc[referencer % MAX_REF] = bloc;
	return (referencer++);
}

void				dynamicMemoryManager::clear_data(uint32_t ref, uint32_t type_id)
{
	allocator[type_id].mem_free(ref_to_bloc[ref % MAX_REF]);
}

void 				*dynamicMemoryManager::get_ptr(uint32_t ref)
{
	return (ref_to_ptr[ref % MAX_REF]);
}

void				dynamicMemoryManager::zerofy(uint32_t handler, uint32_t size)
{
	bzero(get_ptr(handler), size);
}