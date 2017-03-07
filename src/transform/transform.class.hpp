#ifndef TRANSFORM_CLASS_HPP
#define TRANSFORM_CLASS_HPP

#include <glm.hpp>
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <dynamicMemoryManager.class.hpp>
#include <gtc/matrix_transform.hpp>
#include <cstring>
#include <iostream>

#define GLM_FORCE_AVX2

typedef struct 		s_transform {

	bool 			has_parent;
	uint32_t		parentHandler;
	glm::vec3		position;
	glm::vec3 		scale;
	glm::quat 		rotation;
}					t_transform;

class transformBuiltin {

public:

	static void init();
	static void shutdown();

	static uint32_t create();
	static void destroy(uint32_t handler);

	static glm::vec3 get_direction(uint32_t handler);
	static void rotate(uint32_t handler, glm::vec3 axis, float angle);
	static void rotate_model(uint32_t handler, glm::vec3 axis, float angle);
	static void translate(uint32_t handler, float x, float y, float  z);
	static void scale(uint32_t handler, float x, float y, float  z);
	static void euler_angle(uint32_t handler, float x, float y);

	static void grow(uint32_t handler, float factor);

	static glm::mat4 to_mat(uint32_t handler);
	static t_transform *get_transform(uint32_t h);
	static glm::mat4 to_mat_cam(uint32_t handler);
	static glm::mat4	projection_matrix(float fov, float near, float far, float aspect);

	static void			init_rotation(uint32_t handler);

	static void CreateFromAxisAngle(uint32_t handler, float angle);

private:
	static uint32_t 	cluster_id;
};

#endif