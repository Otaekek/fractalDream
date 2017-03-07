#ifndef BASICFPSCONTROLBUILTIN_CLASS_HPP
#define BASICFPSCONTROLBUILTIN_CLASS_HPP

/*
	This factory was created mainly for debug and testing
	As it has no physics components movement are based directly on the transform
	and this is NOT recommended in a real game.
*/

/*
	Note:
		The way the Gameobject are conceived ( if they are ) are left to the game programmer
		but it is recommanded to architect them this way.
*/

#include <shutdown.class.hpp>
#include <transform.class.hpp>
#include <string>
#include <renderBuiltIn.class.hpp>
#include <fileLoader.class.hpp>
#include <inputBuiltin.class.hpp>

#define MAX_ELEM 64

class basicFPSControl {

public:

	basicFPSControl();
	basicFPSControl(float posx, float posy, float posz, float camxpos, float camypos, float camxsize, float camysize);
	~basicFPSControl();

	void		mouse_update(float posX, float posY);
	void 		update();
	uint32_t	_transformHandler;

private:

	uint32_t	_cameraHandler;

	void 		render();
	void 		behave();
	float 		relx;
	float 		rely;
	float		rotx;
	float		roty;
};

class basicFPSControlManagerBuiltin {

public:

	static void 		init();
	static void 		shutdow();

	static void 		update();
	static void 		create(float posx, float posy, float posz, float camxpos, float camypos, float camxsize, float camysize);
	static void 		mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
	static uint32_t		get_main_handler();

private:

	static basicFPSControl		elems[MAX_ELEM];
	static uint32_t				numElem;
};

#endif
