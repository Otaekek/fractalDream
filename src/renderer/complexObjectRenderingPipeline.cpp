#include "renderBuiltIn.class.hpp"


void			renderBuiltIn::render_mesh(t_renderMeshData *mesh, t_renderGO *elem, uint32_t program)
{
	GLuint location;
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mesh->vaoId);
	location = glGetUniformLocation(program, "diffuse");
	glUniform3f(location, mesh->material.diffuse[0], mesh->material.diffuse[1], mesh->material.diffuse[2]);
	location = glGetUniformLocation(program, "specular");
	glUniform3f(location, mesh->material.specular[0], mesh->material.specular[1], mesh->material.specular[2]);
	location = glGetUniformLocation(program, "ambiant");
	glUniform3f(location, mesh->material.ambiant[0], mesh->material.ambiant[1], mesh->material.ambiant[2]);
	location = glGetUniformLocation(program, "has_diffuse");
	glUniform1i(location, (int)mesh->material.has_diffuse_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indiceBufferId);
	if (mesh->material.has_diffuse_texture)
		glBindTexture(GL_TEXTURE_2D, mesh->material.diffuseTexture);
	glDrawElements(GL_TRIANGLES, mesh->indiceNum, GL_UNSIGNED_INT, 0);
}

bool 			cull_light(t_light *light, t_transform *transform)
{
	t_transform *l_pos;

	l_pos = transformBuiltin::get_transform(light->transformHandler);
	if (glm::distance(l_pos->position, transform->position) > light->constantEuristicOcclusion)
		return (false);
	return (true);
}

void			renderBuiltIn::push_light(t_renderGO *elem, GLuint program)
{
	float			array[256];
	t_light			*light;
	t_transform		*transform;
	glm::vec3		direction;
	GLint			location;

	for (int i = 0; i < _numLight; i++)
	{
		light = get_light(_lightsHandlers[i]);
		transform = transformBuiltin::get_transform(light->transformHandler);
		//if (!cull_light(light, transformBuiltin::get_transform(elem->transformHandler)))
		//	continue ;
		direction = transformBuiltin::get_direction(light->transformHandler);
		array[i * 12] = transform->position.x;
		array[i * 12 + 1] = transform->position.y;
		array[i * 12 + 2] = transform->position.z;
		array[i * 12 + 3] = light->color.x;
		array[i * 12 + 4] = light->color.y;
		array[i * 12 + 5] = light->color.z;
		array[i * 12 + 6] = direction.x;
		array[i * 12 + 7] = direction.y;
		array[i * 12 + 8] = direction.z;
	}
	location = glGetUniformLocation(program, "num_light");
	glUniform1i(location, _numLight);
	location = glGetUniformLocation(program, "lights");
	glUniform3fv(location, _numLight * 4, array);
}

void			renderBuiltIn::render_node(t_node node, t_renderGO *elem, uint32_t program)
{
	t_renderMeshData	*mesh;
	bool				mesh_has_child;

	if (node.has_mesh)
	{
		mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(node.meshs);
		do
		{
			render_mesh(mesh, elem, program);
			mesh_has_child = mesh->has_child;
			mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(mesh->child);
		}	while (mesh_has_child);
	}
	for (int i = 0; i < node.childNum; i++)
		render_node(*(t_node*)(staticMemoryManager::get_data_ptr(node.child[i])), elem, program);
}

bool			cull_mesh(glm::mat4 modelMat, glm::mat4 cameraMat, float maxDist, glm::mat4 perspectiveMat)
{
	glm::vec3 dir, axis = glm::vec3(0, 0, -1);
	modelMat *= cameraMat;
	if (modelMat[2][3] > maxDist)
		return (false);
	if (-modelMat[2][3] > 100000)
		return (false);
	dir.x = modelMat[0][3];
	dir.y = modelMat[1][3];
	dir.z = modelMat[2][3];
	if (dir.z > -maxDist * 2)
		return (true);
	if (glm::dot(glm::normalize(dir), axis) < 0.75)
		return (false);
	axis.z = 0;
	axis.y = 1;
	if (glm::dot(glm::normalize(dir), axis) > 0.5)
		return (false);
	return (true);
}

void 			renderBuiltIn::render_object(uint32_t index, t_camera *camera)
{
	t_renderMeshData	*mesh;
	t_node				*node;
	bool				mesh_has_child;
	bool				node_has_child;
	t_renderGO			*elem;
	glm::mat4 			modelMat, projMat, viewMat;

	elem = (t_renderGO*)renderBuiltIn::get_renderGO(index);
	if (!(elem->cameraLayer & camera->cameraLayer) && !((elem->cameraLayer + camera->cameraLayer) == 0))
		return ;
	node = (t_node*)staticMemoryManager::get_data_ptr(elem->assetHandler);
	modelMat = transformBuiltin::to_mat(elem->transformHandler);
	viewMat = transformBuiltin::to_mat_cam(camera->transformHandler);
	projMat = transformBuiltin::projection_matrix(60.0f, 10.0f, 10000000000.0f,
		(float)(mode->width * camera->sizex) / (mode->height * camera->sizey));

	glUseProgram(node->program);

	face_culling(elem);

	push_light(elem, node->program);

	/*Set projection Matrix*/
	GLint uniProj = glGetUniformLocation(node->program, "P");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMat));

	/*Set camera */
	GLint cam = glGetUniformLocation(node->program, "V");
	glUniformMatrix4fv(cam, 1, GL_FALSE, glm::value_ptr(viewMat));

	/* Set model */
	GLint model = glGetUniformLocation(node->program, "M");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelMat));

	//if (cull_mesh(modelMat, viewMat, 500, projMat))
		render_node(*node, elem, node->program);
}
