#include "mapEditorBuiltin.class.hpp"

uint32_t	mapEditorBuiltin::assets[4096];
uint32_t	mapEditorBuiltin::asset_size = 0;
uint32_t	mapEditorBuiltin::position = 0;
uint32_t	mapEditorBuiltin::cameras[4096];
int32_t		mapEditorBuiltin::activeRenderGo = 0;
bool		mapEditorBuiltin::is_active = false;
uint32_t	mapEditorBuiltin::camHandler = 0;
uint32_t	mapEditorBuiltin::_transformHandler = 0;
float		mapEditorBuiltin::smoothPosition = 0;

void mapEditorBuiltin::init()
{
	t_renderGO *rg;

	activeRenderGo = renderBuiltIn::create();
	rg = renderBuiltIn::get_renderGO(activeRenderGo);
	rg->transformHandler = transformBuiltin::create();
}

void set_active_render_go(uint32_t active, uint32_t n)
{
	t_renderGO *a;
	t_renderGO *b;

	a = renderBuiltIn::get_renderGO(active);
	b = renderBuiltIn::get_renderGO(n);
	a->assetHandler = b->assetHandler;
}

void mapEditorBuiltin::update_keyboard()
{
	t_renderGO *renderGO;

	if (is_active)
		renderGO = renderBuiltIn::get_renderGO(activeRenderGo);
	if (inputBuiltin::key_pressed[GLFW_KEY_C])
	{
		position = (position + 1) > asset_size - 1 ? asset_size - 1: position + 1;
		if (is_active && staticMemoryManager::get_asset_state(renderBuiltIn::get_renderGO(assets[position])->assetHandler) == staticMemoryManager::E_LOADED)
			renderGO->assetHandler = renderBuiltIn::get_renderGO(assets[position])->assetHandler;
		inputBuiltin::key_pressed[GLFW_KEY_C] = 0;
	}
	if (inputBuiltin::key_pressed[GLFW_KEY_X])
	{
		position = (int)position - 1 > 0 ? position -1 : 0;
		if (is_active &&
			staticMemoryManager::get_asset_state(renderBuiltIn::get_renderGO(assets[position])->assetHandler) == staticMemoryManager::E_LOADED)
			renderGO->assetHandler = renderBuiltIn::get_renderGO(assets[position])->assetHandler;
		inputBuiltin::key_pressed[GLFW_KEY_X] = 0;
	}
	if (inputBuiltin::key_pressed[GLFW_MOUSE_BUTTON_1])
	{
		if (!is_active)
		{
			if (asset_size > 0 && staticMemoryManager::get_asset_state(renderBuiltIn::get_renderGO(assets[position])->assetHandler) == staticMemoryManager::E_LOADED)
			{
				is_active = true;
				set_active_render_go(activeRenderGo, assets[position]);
			}
		}
		else
		{
			is_active = false;
			staticMeshManager::create_from_handler(renderGO->assetHandler, renderGO->transformHandler);
		}
		inputBuiltin::key_pressed[GLFW_MOUSE_BUTTON_1] = 0;
	}
	if (inputBuiltin::key_pressed[GLFW_MOUSE_BUTTON_2])
	{
		is_active = false;
	}
	if (is_active)
	{
		if (inputBuiltin::key_pressed[GLFW_KEY_F])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(1, 0, 0), 0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_G])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(0, 1, 0), 0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_H])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(0, 0, 1), 0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_R])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(1, 0, 0), -0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_T])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(0, 1, 0), -0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_Y])
		{
			transformBuiltin::rotate_model(renderGO->transformHandler, glm::vec3(0, 0, 1), -0.01);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_U])
		{
			transformBuiltin::init_rotation(renderGO->transformHandler);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_I])
		{
			transformBuiltin::grow(renderGO->transformHandler, 1.1);
		}
		if (inputBuiltin::key_pressed[GLFW_KEY_K])
		{
			transformBuiltin::grow(renderGO->transformHandler, 1.0f / 1.1);
		}
	}
}

static void shift_elems(uint32_t *assets, uint32_t index, uint32_t size)
{
	for (; index < size; index++)
		assets[index] = assets[index + 1];
}

void mapEditorBuiltin::update_ui()
{
	t_camera		*camera;
	t_renderGO		*renderGO;

	for (float i = 0; i < asset_size; i += 1)
	{
		float diff;

		diff = i - smoothPosition;
		if (abs(diff) > 4)
			continue ;
		camera = (t_camera*)renderBuiltIn::get_camera(cameras[(int)i]);
		camera->sizex = 0.2 / (1 + fabs(diff));
		camera->sizey = 0.2 / (1 + fabs(diff));
		camera->posx = 0.5 + ((float)diff / 10) - camera->sizex / 2;
		camera->posy = 0.8;
		renderGO = renderBuiltIn::get_renderGO(assets[(int)i]);
		if ((int)i == position)
			transformBuiltin::rotate(renderGO->transformHandler, glm::vec3(0, 1, 0), 0.01);	
		if (staticMemoryManager::get_asset_state(renderGO->assetHandler) == staticMemoryManager::E_LOADED)
			renderBuiltIn::render_me(assets[(int)i]);
		else if (staticMemoryManager::get_asset_state(renderGO->assetHandler) == staticMemoryManager::E_ERR)
		{
			shift_elems(cameras, (int)i, asset_size);
			shift_elems(assets, (int)i, asset_size--);
			continue ;
		}
		renderBuiltIn::add_camera(cameras[(int)i]);
	}
	float dir = 0;
	if (fabs((float)position - smoothPosition) > 0.01)
	{
		dir = 1.0f / 30;
		if (smoothPosition < position)
			dir *= -1;
		smoothPosition += -dir;
	}
	else
	{
		smoothPosition = position;
	}
}

void mapEditorBuiltin::update_active()
{
	t_renderGO		*renderGO;
	t_transform		*transform;
	t_transform		*camTransform;
	uint32_t		camTransformHandler;
	glm::vec3 		dir;

	if (is_active)
	{	
		camTransformHandler = basicFPSControlManagerBuiltin::get_main_handler();
		renderGO = renderBuiltIn::get_renderGO(activeRenderGo);
		transform = transformBuiltin::get_transform(renderGO->transformHandler);
		dir = transformBuiltin::get_direction(camTransformHandler);
		camTransform = transformBuiltin::get_transform(camTransformHandler);
		transform->position.x = -camTransform->position.x;
		transform->position.y = -camTransform->position.y;
		transform->position.z = -camTransform->position.z;
		dir = glm::normalize(dir);
		transformBuiltin::translate(renderGO->transformHandler, dir.x * -700, dir.y * -700, dir.z * -700);
		/*transformBuiltin::init_rotation(renderGO->transformHandler);
		dir.y = 0;
		dir = glm::normalize(dir);
		float rot = acos((glm::dot(glm::vec3(0, 0, 1), dir)));
		if (dot(dir, glm::vec3(-1, 0, 0)) < 0)
			rot = -rot;
		transformBuiltin::rotate(renderGO->transformHandler, glm::vec3(0, 1, 0), rot);
		printf("%f\n", acos((glm::dot(glm::vec3(0, 0, -1), -dir))) * 360 / (2 * 3.14));
		*/
		renderBuiltIn::render_me(activeRenderGo);
	}
}

void mapEditorBuiltin::update()
{
	update_ui();
	update_keyboard();
	update_active();
}

void mapEditorBuiltin::shutdown()
{

}

void mapEditorBuiltin::push_elem(uint32_t assetHandler)
{
	t_renderGO 		*renderGO;
	t_transform 	*camTransform;
	t_camera		*camera;

	assets[asset_size] = renderBuiltIn::create();
	cameras[asset_size] = renderBuiltIn::create_camera();
	renderGO = renderBuiltIn::get_renderGO(assets[asset_size]);
	camera = (t_camera*)dynamicMemoryManager::get_ptr(cameras[asset_size]);
	camera->transformHandler = transformBuiltin::create();
	camTransform = (t_transform*)dynamicMemoryManager::get_ptr(camera->transformHandler);
	renderGO->assetHandler = assetHandler;
	camera->cameraLayer = 2 << asset_size;
	renderGO->cameraLayer = 2 << asset_size;
	renderGO->transformHandler = transformBuiltin::create();
	transformBuiltin::translate(camera->transformHandler, 0, 0, -800);
	asset_size++;
}
