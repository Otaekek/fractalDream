#include "entitySystem.class.hpp"

uint32_t		entitySystem::_entitys[MAX_ENTITY];
uint32_t		entitySystem::_numEntity;

uint32_t		entitySystem::_clusterId;

void entitySystem::init()
{
	_numEntity = 0;
	_clusterId = dynamicMemoryManager::init_cluster(sizeof(entity), MAX_ENTITY);
}

void entitySystem::shutdown()
{

}

void entitySystem::update()
{
	for (uint32_t i = 0; i < _numEntity; i++)
		(entity*)dynamicMemoryManager::get_data_ptr(_entitys[i])->update();
}

void entitySystem::push(uint32_t handler)
{
	(entity*)dynamicMemoryManager::get_data_ptr(_entitys[i])->set_index(_numEntity);
	_entitys[_numEntity++] = handler;
}

void entitySystem::destroy(uint32_t handler)
{
	uint32_t id;

	id = (entity*)dynamicMemoryManager::get_data_ptr(_entitys[handler])->get_index();
	dynamicMemoryManager::free_slot(handler);
	_entitys[id] = _entitys[--_numEntity];
	(entity*)dynamicMemoryManager::get_data_ptr(_entitys[id])->set_index(id);
}

