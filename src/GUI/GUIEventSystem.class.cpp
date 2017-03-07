#include "GUIEventSystem.class.hpp"

GUIEventSystem::GUIEventSystem()
{
	is_hoover = false;
	_keyboardEventIndex = 0;
	_GUIid = 0;
}

GUIEventSystem::GUIEventSystem(uint32_t id)
{
	is_hoover = false;
	_keyboardEventIndex = 0;
	_mouseEventIndex = 0;
	_GUIid = id;
}

GUIEventSystem::~GUIEventSystem()
{

}

void GUIEventSystem::update()
{
	check_event();
}

void GUIEventSystem::set_GUIid(uint32_t GUIid)
{
	_GUIid = GUIid;
}

void GUIEventSystem::check_keyboard()
{
	for (uint32_t i = _keyboardEventIndex; i != inputBuiltin::_keyboardEventNum; i = (i + 1) % inputBuiltin::_keyboardEventSize)
		onKeyBoardEvent(inputBuiltin::_keyboardEvents[i]);
	_keyboardEventIndex = inputBuiltin::_keyboardEventNum;
}

void GUIEventSystem::check_mouse()
{
	int posx = inputBuiltin::_mouseXPos;
	int posy = inputBuiltin::_mouseYPos;

	// if (!renderer::has_gui_ref(_GUIidposx, posy))
	//
	for (uint32_t i = _mouseEventIndex; i != inputBuiltin::_mouseEventNum; i = (i + 1) % inputBuiltin::_mouseEventSize)
		onMouseEvent(inputBuiltin::_mouseEvents[i]);
	_mouseEventIndex = inputBuiltin::_mouseEventNum;
	onMouseMoveEvent(posx, posy);
}

void GUIEventSystem::check_event()
{
	check_keyboard();
	check_mouse();
}

void GUIEventSystem::onMouseEvent(t_mouseEvent event)
{
	printf("Mouse clicked\n");
}

void GUIEventSystem::onMouseMoveEvent(int posx, int posy)
{
	printf("Mouse moved\n");
}

void GUIEventSystem::onMouseHooverEvent(int posx, int posy)
{
	printf("Mouse hoover\n");
}

void GUIEventSystem::onMouseLeaveEvent(int posx, int posy)
{
	printf("Mouse leave\n");
}

void GUIEventSystem::onKeyBoardEvent(t_keyboardEvent event)
{
	printf("%c %d\n", event.key, event.action);
	printf("Key pressed\n");
}
