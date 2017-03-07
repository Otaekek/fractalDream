#include "memory_allocator/stackAllocator.class.hpp"
#include "static_memory_manager/staticMemoryManager.class.hpp"
#include "job_system/jobSystem.hpp"
#include "file_loader/fileLoader.class.hpp"
#include <cstdlib>
#include <iostream>
#include "job_system/jobHandler.class.hpp"
#include "../assimp/include/assimp/Importer.hpp"
#include <unistd.h>
#include "memory_allocator/poolAllocator.class.hpp"
#include "dynamic_memory_manager/dynamicMemoryManager.class.hpp"
#include <transform.class.hpp>
#include <iostream>
#include <glfw3.h>
#include <list>
#include <vector>
#include <ctime>
#include <renderBuiltIn.class.hpp>
#include <basicFPSControlBuiltin.class.hpp>
#include <inputBuiltin.class.hpp>
#include <texture_builtin.class.hpp>
#include <basicMeshFactory.class.hpp>
#include <basicLightFactory.class.hpp>
#include <terrainGenerationBuiltin.class.hpp>
#include <mapEditorBuiltin.class.hpp>
#include <directorySmartLoader.class.hpp>
#include <runtimeLibrary.class.hpp>
#include <GUIEventSystem.class.hpp>

void init()
{
	texture_builtin::init();
	transformBuiltin::init();
	renderBuiltIn::init();
	fileLoader::init();
	jobHandler::init();
	renderDataSys::init();
	inputBuiltin::init();
	basicFPSControlManagerBuiltin::init();
	mapEditorBuiltin::init();
}

void shutdown()
{
	transformBuiltin::shutdown();
	jobHandler::shutdown();
}

void update()
{
	inputBuiltin::update();
	basicFPSControlManagerBuiltin::update();
	staticMeshManager::update();
	terrainGenerationBuiltin::update();
	basicLightFactory::update();
	mapEditorBuiltin::update();
	renderBuiltIn::update();
}

int main()
{
	GUIEventSystem elem(3);
	init();
	runtimeLibrary::create_lib("assets/levelBuilder/startEngine");
	while (42)
	{
		update();
		elem.update();
	}
	shutdown();
	return (0);
}
