#ifndef GUIEVENTSYSTEM_CLASS_HPP
#define GUIEVENTSYSTEM_CLASS_HPP

#include <inputBuiltin.class.hpp>

class GUIEventSystem {

public:

	GUIEventSystem();
	GUIEventSystem(uint32_t id);
	~GUIEventSystem();

	void update();

	void set_GUIid(uint32_t GUIid);

private:

	void check_event();
	void check_keyboard();
	void check_mouse();

	virtual void onMouseEvent(t_mouseEvent event);
	virtual void onMouseMoveEvent(int posx, int posy);
	virtual void onMouseHooverEvent(int posx, int posy);
	virtual void onMouseLeaveEvent(int posx, int posy);
	virtual void onKeyBoardEvent(t_keyboardEvent event);

	bool		is_hoover;
	uint32_t	_keyboardEventIndex;
	uint32_t	_mouseEventIndex;
	uint32_t	_GUIid;
};
#endif