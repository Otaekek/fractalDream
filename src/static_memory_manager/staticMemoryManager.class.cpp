#include "staticMemoryManager.class.hpp"

stackAllocator											staticMemoryManager::clusters[NUMCLUSTER];
staticMemoryManager::e_asset_state						staticMemoryManager::data_status[MAXREF];
void													*staticMemoryManager::ref_to_ptr[MAXREF] = {0};
uint32_t	 											staticMemoryManager::referencer = 1;
uint32_t												staticMemoryManager::count = 0;
std::mutex												staticMemoryManager::mutexes[NUMCLUSTER];
uint32_t												staticMemoryManager::clusters_count = 0;
uint32_t												staticMemoryManager::ref_to_cluster[MAXREF];

void 										staticMemoryManager::shutdown()
{
	for (uint32_t i = 0; i < NUMCLUSTER; i++)
		clusters[i].all_mem_free();
}

uint32_t									staticMemoryManager::init_cluster()
{
	clusters_count %= NUMCLUSTER;
	return (clusters_count++);
}

staticMemoryManager::e_asset_state			staticMemoryManager::get_asset_state(uint32_t ref)
{
	return (data_status[ref]);
}

void 										staticMemoryManager::set_asset_state(staticMemoryManager::e_asset_state state, uint32_t ref)
{
	if (state == E_ERR || state == E_LOADED)
		count--;
	data_status[ref] = state;
}

void 										*staticMemoryManager::get_data_ptr(uint32_t ref)
{
	return (ref_to_ptr[ref]);
}

void 										staticMemoryManager::free_all()
{
	for (uint16_t i = 0; i < NUMCLUSTER; i++)
		clusters[i].all_mem_free();
}

bool										staticMemoryManager::all_read()
{
	return (count == 0);
}

uint32_t									staticMemoryManager::create_asset(uint32_t cluster_id, uint32_t size)
{
	mutexes[0].lock();
	referencer++;
	ref_to_ptr[referencer] = clusters[cluster_id].mem_alloc(size);
	ref_to_cluster[referencer] = cluster_id;
	mutexes[0].unlock();
	return (referencer);
}

void										staticMemoryManager::realloc(uint32_t size, uint32_t cluster_id)
{
	uint64_t	offsetOld;
	uint64_t	offsetNew;
	uint32_t	old_size;

	/*
	return ;
	old_size = clusters[cluster_id].get_size();
	offsetOld = (uint64_t)clusters[cluster_id].get_data_pointer();
	clusters[cluster_id].all_mem_free();
	clusters[cluster_id].init((size + old_size) * 2);
	offsetNew = (uint64_t)clusters[cluster_id].get_data_pointer();
	for (uint32_t i = 1; ref_to_ptr[i]; i++)
	{
		if (ref_to_cluster[i] == cluster_id)
			ref_to_ptr[i] += offsetNew - offsetOld;
	}
	*/
}

uint32_t									staticMemoryManager::assign_asset(uint32_t cluster)
{
	mutexes[0].lock();
	referencer++;
	data_status[referencer] = E_LOADING;
	ref_to_cluster[referencer] = cluster;
	mutexes[0].unlock();
	return (referencer);
}

void										*staticMemoryManager::alloc_asset(uint32_t ref, uint32_t size)
{
	mutexes[0].lock();
	ref_to_ptr[ref] = clusters[ref_to_cluster[ref]].mem_alloc(size);
	mutexes[0].unlock();
	return (ref_to_ptr[ref]);
}