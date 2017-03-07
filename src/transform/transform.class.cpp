#include <transform.class.hpp>
#include <gtc/type_ptr.hpp>
uint32_t transformBuiltin::cluster_id = 0;

void transformBuiltin::init()
{
	cluster_id = dynamicMemoryManager::cluster_init(sizeof(t_transform), 65536);
}

void transformBuiltin::shutdown()
{

}

uint32_t transformBuiltin::create()
{
	t_transform *transform;
	uint32_t ref = (dynamicMemoryManager::create_slot(cluster_id));

	transform = get_transform(ref);
	memset(transform, 0, sizeof(t_transform));
	scale(ref, 1, 1, 1);
	transform->rotation = glm::angleAxis(0.0f, glm::vec3{0.0f, 1.0f, 0.0f});
	return (ref);
}

void transformBuiltin::destroy(uint32_t handler)
{
	dynamicMemoryManager::clear_data(handler, cluster_id);
}

void transformBuiltin::grow(uint32_t handler, float factor)
{
	t_transform *transform;

	transform = get_transform(handler);
	transform->scale *= factor;
}

glm::vec3 transformBuiltin::get_direction(uint32_t handler)
{

	t_transform *transform;
	glm::vec3 axis = {0, 0, 1};

	transform = get_transform(handler);
	axis = transform->rotation * axis;
	return (axis);
}

void transformBuiltin::rotate_model(uint32_t handler, glm::vec3 axis, float angle)
{
	t_transform *transform;

	transform = get_transform(handler);
	transform->rotation = glm::rotate(transform->rotation, angle, axis);
}

void transformBuiltin::rotate(uint32_t handler, glm::vec3 axis, float angle)
{
	t_transform *transform;

	transform = get_transform(handler);
	axis = transform->rotation * axis;
	transform->rotation = glm::rotate(transform->rotation, angle, axis);
}

void transformBuiltin::translate(uint32_t handler, float x, float y, float z)
{
	t_transform *transform;

	transform = get_transform(handler);
	transform->position.x += x;
	transform->position.y += y;
	transform->position.z += z;

}

void transformBuiltin::scale(uint32_t handler, float x, float y, float  z)
{
	t_transform *transform;

	transform = get_transform(handler);
	transform->scale.x = x;
	transform->scale.y = y;
	transform->scale.z = z;
}

t_transform *transformBuiltin::get_transform(uint32_t h)
{
	t_transform *ret;

	ret = (t_transform*)dynamicMemoryManager::get_ptr(h);
	return (ret);
}

glm::mat4 transformBuiltin::to_mat(uint32_t handler)
{
	t_transform *transform;
	glm::mat4 ret;

	transform = get_transform(handler);
	glm::mat4 scale = glm::scale(glm::mat4x4(1.0f), transform->scale);
	ret = scale;
	ret *= (glm::mat4_cast(transform->rotation));
	ret[0][3] = transform->position.x;
	ret[1][3] = transform->position.y;
	ret[2][3] = transform->position.z;
	return (ret);
}


glm::mat4 transformBuiltin::to_mat_cam(uint32_t handler)
{
	t_transform *transform;
	glm::mat4 ret;

	transform = get_transform(handler);
	glm::mat4 scale = glm::scale(glm::mat4x4(1.0f), transform->scale);
	ret = scale;
	ret[0][3] = transform->position.x;
	ret[1][3] = transform->position.y;
	ret[2][3] = transform->position.z;
	ret *= (glm::mat4_cast(transform->rotation));
	return (ret);
}


glm::mat4	transformBuiltin::projection_matrix(float fov, float near, float far, float aspect)
{
	glm::mat4 	matrix;
	float		x_scale;
	float		y_scale;
	float		frustrum;

	matrix = glm::mat4x4(1.0f);
	y_scale = (float)((1.0f / tan((fov / 2.0f) / 57.295)) * aspect);
	x_scale = y_scale / aspect;
	frustrum = far - near;
	matrix[0][0] = x_scale;
	matrix[1][1] = y_scale;
	matrix[2][2] = -((far + near) / frustrum);
	matrix[2][3] = -((2 * near * far) / frustrum);
	matrix[3][2] = -1;
	matrix[3][3] = 0;
	return (matrix);
}

void transformBuiltin::euler_angle(uint32_t handler, float x, float y)
{
	t_transform *transform;

	transform = get_transform(handler);
	transform->rotation = glm::angleAxis(y, glm::vec3{1.0f, 0.0f, 0.0f});
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	up = up * transform->rotation;
	transform->rotation = glm::rotate(transform->rotation, x, up);
}

void		transformBuiltin::init_rotation(uint32_t handler)
{
	t_transform *transform = get_transform(handler);
	transform->rotation = glm::angleAxis(0.0f, glm::vec3{0.0f, 1.0f, 0.0f});
}

void 	transformBuiltin::CreateFromAxisAngle(uint32_t handler, float angle)
{
	t_transform *transform;

	transform = get_transform(handler);
    float halfAngle = angle * .5f;
    float s = sin(halfAngle);
    transform->rotation.x = 0 * s;
    transform->rotation.y = 1 * s;
    transform->rotation.z = 0 * s;
    transform->rotation.w = cos(halfAngle);
}