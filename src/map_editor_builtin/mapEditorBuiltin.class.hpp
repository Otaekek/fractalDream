#ifndef MAPEDITORBUILTIN_CLASS_HPP
#define MAPEDITORBUILTIN_CLASS_HPP

#include <renderBuiltIn.class.hpp>
#include <inputBuiltin.class.hpp>
#include <staticMemoryManager.class.hpp>
#include <shutdown.class.hpp>
#include <basicFPSControlBuiltin.class.hpp>
#include <basicMeshFactory.class.hpp>

class mapEditorBuiltin {

public:

	static void init();

	static void update();

	static void shutdown();

	static void push_elem(uint32_t assetHandler);

	static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
private:

	static uint32_t assets[4096];
	static uint32_t	cameras[4096];
	static uint32_t	asset_size;
	static uint32_t	position;
	static float	smoothPosition;
	static int32_t	activeRenderGo;
	static uint32_t	camHandler;
	static bool		is_active;
	static uint32_t	_transformHandler;

	static void update_keyboard();
	static void update_ui();
	static void update_active();
};

#endif
