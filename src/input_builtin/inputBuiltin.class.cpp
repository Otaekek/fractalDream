
#include "inputBuiltin.class.hpp"


t_keyboardEvent		inputBuiltin::_keyboardEvents[4096];
uint32_t			inputBuiltin::_keyboardEventNum = 0;
uint32_t			inputBuiltin::_keyboardEventSize = 4096;

t_mouseEvent		inputBuiltin::_mouseEvents[4096];
uint32_t			inputBuiltin::_mouseEventNum = 0;
uint32_t			inputBuiltin::_mouseEventSize = 4096;

bool inputBuiltin::key_pressed[4096] = {false};

float inputBuiltin::_mouseXPos = 0;
float inputBuiltin::_mouseYPos = 0;


float inputBuiltin::_relMouseYPos = 0;
float inputBuiltin::_relMouseXPos = 0;

void inputBuiltin::init()
{
	glfwSetKeyCallback(renderBuiltIn::get_window(), inputBuiltin::key_callback);
	glfwSetMouseButtonCallback(renderBuiltIn::get_window(), inputBuiltin::mouse_button_callback);
	glfwSetCursorPos(renderBuiltIn::get_window(), renderBuiltIn::get_mode()->width / 2, renderBuiltIn::get_mode()->height / 2);
}

void inputBuiltin::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	t_mouseEvent event;

	event.action = action;
	event.button = button;
	event.mod = mods;
	if (action == GLFW_PRESS)
	{
		key_pressed[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		key_pressed[button] = false;
	}
	_mouseEvents[_mouseEventNum] = event;
	_mouseEventNum = (_mouseEventNum + 1) % _mouseEventSize;
}


void inputBuiltin::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_keyboardEvent event;

	if (action == GLFW_PRESS)
	{
		key_pressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		key_pressed[key] = false;
	}
	event.key = key;
	event.scancode = scancode;
	event.action = action;
	event.mod = mods;
	_keyboardEvents[_keyboardEventNum] = event;
	_keyboardEventNum = (_keyboardEventNum + 1) % _keyboardEventSize;
}

void inputBuiltin::shutdown()
{

}

void inputBuiltin::update()
{
	glfwPollEvents();
}

void inputBuiltin::hide_cursor()
{
	glfwSetInputMode(renderBuiltIn::get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void  inputBuiltin::add_mouse_move_callback(void (*f)(GLFWwindow *w, double x, double y))
{
	glfwSetCursorPosCallback(renderBuiltIn::get_window(), f);
}

void inputBuiltin::add_keyboard_callback(void (*f)(GLFWwindow *w, int button, int scancode, int action, int mods))
{
	glfwSetKeyCallback(renderBuiltIn::get_window(), f);
}

void add_mouse_button_callback(GLFWwindow* window, void (*f)(GLFWwindow *w, int button, int action, int mods))
{
	glfwSetMouseButtonCallback(renderBuiltIn::get_window(), f);
}

void inputBuiltin::disable_cursor()
{
	glfwSetInputMode(renderBuiltIn::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
