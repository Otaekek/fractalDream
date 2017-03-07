#include <stdio.h>
#include <mapEditorBuiltin.class.hpp>
#include <fileLoader.class.hpp>
#include <shutdown.class.hpp>
#include <runtimeLibrary.class.hpp>
#include <basicLightFactory.class.hpp>
#include <directorySmartLoader.class.hpp>
#include <GUIEventSystem.class.hpp>

void debut()
{
	uint32_t asset = fileLoader::load_fs_asset_sync("./assets/graphic/mesh/cube/cube.obj", E_3D);
	fileLoader::load_fs_asset_sync("./assets/graphic/mesh/church/mdr.obj", E_3D);

	renderDataSys::set_programm(E_SKYBOX, asset);
	renderBuiltIn::modify_skybox(asset);
	basicFPSControlManagerBuiltin::create(0, 0, -2, 0, 0, 1, 1);
	renderBuiltIn::modify_skybox_light(1.2);
	
	basicLightFactory::create(glm::vec3(0, 10000, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), E_UNI, glm::vec3(1 * 2, 1 * 2, 1 * 2));
	staticMeshManager::create(glm::vec3(0, -20, -100), glm::vec3(0, 1, 0), 0, glm::vec3(1, 1 , 1), "./assets/graphic/mesh/church/mdr.obj");
}
