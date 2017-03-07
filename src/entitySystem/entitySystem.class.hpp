#ifndef ENTITYSYSTEM_CLASS_HPP
#define ENTITYSYSTEM_CLASS_HPP

#define MAX_ENTITY 65536 * 2

#include <dynamicMemoryManager.class.hpp>
#include "entity.class.hpp"

class entitySystem {

public:

	void init();
	void shutdown();
	void update();

	void push(uint32_t handler);
	void destroy(handler);

private:

	static uint32_t		_entitys[MAX_ENTITY];
	static uint32_t		_numEntity;

	static uint32_t		_cluster_id;

};

#endif