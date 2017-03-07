#include "basicFPSControlBuiltin.class.hpp"

basicFPSControl::basicFPSControl()
{

}

basicFPSControl::basicFPSControl(float posx, float posy, float posz, float camxpos, float camypos, float camxsize, float camysize)
{
	t_camera *camera;

	_cameraHandler = renderBuiltIn::create_camera();
	camera = renderBuiltIn::get_camera(_cameraHandler);
	camera->posx = camxpos;
	camera->posy = camypos;
	camera->sizex = camxsize;
	camera->sizey = camysize;
	camera->transformHandler = transformBuiltin::create();
	_transformHandler = camera->transformHandler;
	rotx = 0;
	roty = 0;
	relx = 0;
	rely = 0;
	transformBuiltin::translate(_transformHandler, posx, posy, posz);
	inputBuiltin::disable_cursor();
}

basicFPSControl::~basicFPSControl()
{

}

void 		basicFPSControl::update()
{
	render();
	behave();
}

void 		basicFPSControl::render()
{
	renderBuiltIn::add_camera(_cameraHandler);
}

void		basicFPSControl::mouse_update(float mouseX, float mouseY)
{
	mouseX = mouseX / (renderBuiltIn::get_mode()->width) - 0.5;
	mouseY = mouseY / (renderBuiltIn::get_mode()->height) - 0.5;
	rotx -= mouseX - relx;
	roty -= mouseY - rely;
	if (roty < -0.8)
		roty = -0.8;
	if (roty > 0.8)
		roty = 0.8;
	rely = mouseY;
	relx = mouseX;
	transformBuiltin::euler_angle(_transformHandler, rotx, roty);
}

void 		basicFPSControl::behave()
{
	glm::vec3 up = glm::vec3(0, 1, 0);

	glm::vec3 direction = transformBuiltin::get_direction(_transformHandler);
	glm::vec3 crossDirection = -cross(up, direction);

	crossDirection *= 0.2;
	direction *= 0.2;
	if (inputBuiltin::key_pressed[GLFW_KEY_LEFT_SHIFT])
	{
		crossDirection *= 20;
		direction *= 20;
	}
	if (inputBuiltin::key_pressed[GLFW_KEY_W])
		transformBuiltin::translate(_transformHandler, direction.x, direction.y, direction.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_S])
		transformBuiltin::translate(_transformHandler, -direction.x, -direction.y, -direction.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_D])
		transformBuiltin::translate(_transformHandler, crossDirection.x, crossDirection.y, crossDirection.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_A])
		transformBuiltin::translate(_transformHandler, -crossDirection.x, -crossDirection.y, -crossDirection.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_ESCAPE])
		shutdown::exit_jojishi();
}

basicFPSControl		basicFPSControlManagerBuiltin::elems[MAX_ELEM];
uint32_t			basicFPSControlManagerBuiltin::numElem = 0;

void 		basicFPSControlManagerBuiltin::shutdow()
{

}

void 		basicFPSControlManagerBuiltin::update()
{
	for (uint32_t i = 0; i < numElem; i++)
		elems[i].update();
}

void 		basicFPSControlManagerBuiltin::mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	for (uint32_t i = 0; i < numElem; i++)
		elems[i].mouse_update(xpos, ypos);
	//glfwSetCursorPos(renderBuiltIn::get_window(), renderBuiltIn::get_mode()->width / 2, renderBuiltIn::get_mode()->height / 2);
}

void 		basicFPSControlManagerBuiltin::create(float posx, float posy, float posz,float camxpos, float camypos, float camxsize, float camysize)
{
	basicFPSControl control(posx, posy, posz, camxpos, camypos, camxsize, camysize);

	elems[numElem++] = control;
}

void 		basicFPSControlManagerBuiltin::init()
{
	inputBuiltin::add_mouse_move_callback(basicFPSControlManagerBuiltin::mouse_move_callback);
}

uint32_t	basicFPSControlManagerBuiltin::get_main_handler()
{
	return (elems[0]._transformHandler);
}








