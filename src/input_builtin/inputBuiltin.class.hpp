#ifndef INPUTBUILTIN_CLASS_HPP
#define INPUTBUILTIN_CLASS_HPP

#include <glfw3.h>
#include <renderBuiltIn.class.hpp>

typedef struct s_keyboardEvent
{
	int			key;
	int			scancode;
	int			action;
	int			mod;
}				t_keyboardEvent;

typedef struct  s_mouseEvent
{
	int			button;
	int			action;
	int			mod;
}				t_mouseEvent;

class inputBuiltin {

public:

	static void init();
	static void shutdown();

	static void update();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static void hide_cursor();
	static void add_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void add_mouse_move_callback(void (*f)(GLFWwindow *w, double x, double y));
	static void add_keyboard_callback(void (*f)(GLFWwindow *w, int button, int scancode, int action, int mods));
	
	static void disable_cursor();

	static bool					key_pressed[4096];
	static float				_mouseYPos;
	static float				_mouseXPos;
	static float				_relMouseYPos;
	static float				_relMouseXPos;

	static t_keyboardEvent		_keyboardEvents[4096];
	static uint32_t				_keyboardEventNum;
	static uint32_t				_keyboardEventSize;

	static t_mouseEvent		_mouseEvents[4096];
	static uint32_t				_mouseEventNum;
	static uint32_t				_mouseEventSize;

};

#endif