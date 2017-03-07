#ifndef STATICMEMORYMANAGER_CLASS_HPP
#define STATICMEMORYMANAGER_CLASS_HPP

#define NUMCLUSTER 8
#define MAXREF 4096

#include <cstdint>
#include "../memory_allocator/stackAllocator.class.hpp"
#include <cstring>
#include <mutex>

class staticMemoryManager {

public:

	enum e_asset_state {
		E_UNDEF = 0,
		E_LOADING,
		E_LOADED,
		E_CHILD,
		E_ERR
	};

	static void 									shutdown();
	static uint32_t									init_cluster();

	static uint32_t									create_asset(uint32_t cluster_id, uint32_t size);

	static staticMemoryManager::e_asset_state		get_asset_state(uint32_t ref);

	static void 									set_asset_state(staticMemoryManager::e_asset_state state, uint32_t ref);

	static void 									*get_data_ptr(uint32_t ref);

	static void 									free_all();

	static bool										all_read();

	static uint32_t									assign_asset(uint32_t cluster);

	static void										*alloc_asset(uint32_t ref, uint32_t size);

private:

	static stackAllocator							clusters[NUMCLUSTER];
	static staticMemoryManager::e_asset_state		data_status[MAXREF];
	static void										*ref_to_ptr[MAXREF];
	static uint32_t	 								referencer;
	static uint32_t									count;
	static uint32_t									clusters_count;
	static std::mutex								mutexes[NUMCLUSTER];
	static void										realloc(uint32_t size, uint32_t cluster_id);
	static uint32_t									ref_to_cluster[MAXREF];
};

#endif