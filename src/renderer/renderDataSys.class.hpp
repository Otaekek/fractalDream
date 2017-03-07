#ifndef RENDERDATASYS_CLASS_HPP
#define RENDERDATASYS_CLASS_HPP

#include <cstdint>
#include <transform.class.hpp>
#include "../memory_allocator/stackAllocator.class.hpp"
#include "../../assimp/include/assimp/scene.h"
#include "../file_loader/fileLoader.class.hpp"
#include "meshData.hpp"
#include <texture_builtin.class.hpp>
#include <FreeImage.h>
#include <glm.hpp>
#include <cstdlib>
#include <jojishi.hpp>
#include <string>
#include <mutex>
#include <fcntl.h>

enum E_SHADER {
	E_DEFAULT,
	E_SKYBOX,
	E_ONSCREEN,
	E_TESTFRACTAL
};

enum E_LIGHT {
	E_UNI = 0,
	E_OMNI
};

typedef	struct s_texture_request
{
	uint32_t	instanceHandler;
	uint32_t	meshDataHandler;
	uint32_t	mag_filter;
	uint32_t	min_filter;
	uint32_t	wraps_filter;
	uint32_t	fieldID;
	uint32_t	wrapt_filter;	
}				t_texture_request;

typedef struct 	s_request {
	uint32_t	indices_size;
	uint32_t	vertex_size;
	float		*vertex;
	uint32_t	*indices;
	uint32_t	meshDataHandler;
}				t_vao_request;

typedef struct s_light {

	E_LIGHT		light;
	uint32_t	transformHandler;
	glm::vec3	color;
	float		attenuation;
	float		linearAttenuation;
	float		quadraticAttenuation;
	float		constantEuristicOcclusion;

}				t_light;

typedef struct		s_material {

	bool			has_diffuse_texture;
	bool			has_specular_texture;
	bool			has_bump_texture;
	bool			is_twosided;

	float			diffuse[3];
	float			ambiant[3];
	float			specular[3];

	uint32_t		bumpTexture;
	uint32_t		diffuseTexture;
	uint32_t		specularTexture;

}					t_material;

typedef struct		s_mesh {

	bool			has_child;

	GLuint			textureHandler;
	GLuint			vaoId;
	GLuint			vertexNum;
	GLuint			vboVerticeId;
	GLuint			indiceNum;
	GLuint			indiceBufferId;
	GLuint			diffuseText;
	GLuint			specularTexts;

	GLuint			child;

	uint32_t		indices;
	uint32_t 		vbo;

	glm::mat4		initialTransform;
	t_material		material;

}					t_renderMeshData;

typedef	struct		s_node {

	uint32_t		program;
	bool			has_mesh;
	uint32_t		meshs;
	uint32_t 		child[12];
	uint32_t		childNum;

}					t_node;

class renderDataSys {

public:

	renderDataSys();

	~renderDataSys();

	static void 			init();
	static void 			shutdown();

	static uint32_t 		createVAO();
	static uint32_t 		createVBO_VNT(float *vertices, uint32_t vertex_size, uint32_t vaoId);
	static uint32_t 		createVBO_Indice(uint32_t *indices, uint32_t indice_size, uint32_t vaoId);
	static void 			obj_scene_to_memory_as_mesh(const aiScene *scene, char *path, uint32_t ref, uint32_t cluster);
	static uint32_t 		load_programVertexFrag(std::string vertexPath, std::string fragPath);
	static void				computeModification(t_renderMeshData *mesh, glm::vec3 translation, float angle, glm::vec3 axis);
	static void				iterNode(t_node node, glm::vec3 translation, float angle, glm::vec3 axis);
	static void				modifyVertices(uint32_t assetHandler, glm::vec3 translation, float angle, glm::vec3 axis);
	static void				set_programm(E_SHADER shader, uint32_t asset);
	static void				push_request(t_vao_request);
	static void				execute_vao_request();
	static void				push_texture_request(t_texture_request request);
	static void 			execute_texture_request();
	static uint32_t				_programm[16];
	static std::mutex 		mu;
private:
	static void 				copy_vertices(aiMesh *mesh, t_renderMeshData *meshData, const aiScene *scene, char *path, uint32_t meshHandler);
	static uint32_t 			node_to_mesh(const aiNode *node,  glm::mat4 trans,const aiScene *scene, char *path, uint32_t ref, uint32_t cluster);
	static void					handle_texture(aiTextureType type, char *path, aiMaterial *material, uint32_t *textEmplacement, bool *has_text, uint32_t meshDataHandler);
	static t_vao_request		_vao_requests[4096];
	static uint32_t				_vao_request_index;
	static uint32_t				_texture_request_index;
	static t_texture_request	_texture_requests[4096];
};

#endif