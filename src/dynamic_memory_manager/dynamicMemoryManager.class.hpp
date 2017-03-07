#ifndef DYNAMICMEMORYMANAGER
#define DYNAMICMEMORYMANAGER

#include "../memory_allocator/poolAllocator.class.hpp"
#include <cstring>

#define NUM_CLUSTERS 65536
#define MAX_REF 524288

class dynamicMemoryManager {

public:
	static uint32_t 			cluster_init(uint32_t sizeBloc, uint32_t numBloc);
	static uint32_t				create_slot(uint32_t type_id);
	static void					clear_data(uint32_t ref, uint32_t type_id);
	static void 				*get_ptr(uint32_t ref);
	static void					zerofy(uint32_t handler, uint32_t size);

private:
	static poolAllocator 		allocator[NUM_CLUSTERS];
	static uint32_t				referencer;
	static uint32_t				cluster_id;
	static void 				*ref_to_ptr[MAX_REF];
	static void 				*ref_to_bloc[MAX_REF];
};

#endif