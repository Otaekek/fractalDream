#include "renderDataSys.class.hpp"

uint32_t			renderDataSys::_programm[16];
t_vao_request		renderDataSys::_vao_requests[4096];
uint32_t			renderDataSys::_vao_request_index = 0;
std::mutex			renderDataSys::mu;
uint32_t			renderDataSys::_texture_request_index = 0;
t_texture_request	renderDataSys::_texture_requests[4096];

renderDataSys::renderDataSys()
{

}

renderDataSys::~renderDataSys()
{

}

void	renderDataSys::push_texture_request(t_texture_request request)
{
	mu.lock();	
	_texture_requests[++_texture_request_index] = request;
	mu.unlock();
}

void 	renderDataSys::execute_texture_request()
{
	t_renderMeshData 	*meshData;
	t_texture_request	request;

	mu.lock();
	while (_texture_request_index > 0)
	{
		request = _texture_requests[_texture_request_index];
		meshData = (t_renderMeshData*)staticMemoryManager::get_data_ptr(request.meshDataHandler);
		if (request.fieldID == 0)
			meshData->material.diffuseTexture = texture_builtin::convert_to_opengl_parametric(request.instanceHandler,
			request.wraps_filter, request.wrapt_filter,
			request.min_filter, request.mag_filter);
		_texture_request_index--;
	}
	mu.unlock();
}

void	renderDataSys::push_request(t_vao_request request)
{
	mu.lock();
	_vao_requests[++_vao_request_index] = request;
	mu.unlock();
}

void	renderDataSys::execute_vao_request()
{
	t_vao_request 		request;
	uint32_t 			vao;
	t_renderMeshData 	*meshData;

	mu.lock();
	while (_vao_request_index > 0)
	{
		request =_vao_requests[_vao_request_index];
		vao = createVAO();
		meshData = (t_renderMeshData*)staticMemoryManager::get_data_ptr(request.meshDataHandler);
		meshData->vaoId = vao;
		meshData->vboVerticeId = createVBO_VNT(request.vertex, request.vertex_size, vao);
		meshData->indiceBufferId = createVBO_Indice(request.indices, request.indices_size, vao);
		_vao_request_index--;
	}
	mu.unlock();
}

void 			renderDataSys::init()
{
	_programm[0] = load_programVertexFrag("assets/shaders/basicVertexShader.shader", "assets/shaders/basicFragShader.shader");
	_programm[1] = load_programVertexFrag("assets/shaders/skyboxVertexShader.shader", "assets/shaders/skyboxFragShader.shader");
	_programm[2] = load_programVertexFrag("assets/shaders/onScreenVertexShader.shader", "assets/shaders/onScreenFragShader.shader");
	_programm[3] = load_programVertexFrag("assets/shaders/fractals/testFractalVertexShader.shader", "assets/shaders/fractals/testFractalFragShader.shader");
}

void 			renderDataSys::shutdown()
{

}

uint32_t renderDataSys::createVAO()
{
	GLuint vaoid;

	glGenVertexArrays(1, &(vaoid));
	return (vaoid);
}

uint32_t renderDataSys::createVBO_VNT(float *vertices, uint32_t vertex_size, uint32_t vaoId)
{
	GLuint 	vertexBufferID;
	GLuint	normalBufferID;
	GLuint	textCoordBufferID;

	glBindVertexArray(vaoId);

	glGenBuffers(1, &(vertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,  vertex_size * sizeof(float) * 8, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*6));
	return vertexBufferID;
}

uint32_t renderDataSys::createVBO_Indice(uint32_t *indices, uint32_t indice_size, uint32_t vaoId)
{
	GLuint indiceBufferId;
	glBindVertexArray(vaoId);
	glGenBuffers(1, &(indiceBufferId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indice_size * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	return indiceBufferId;
}

static void back_slash(char *s)
{
	for (uint32_t i = 0; s[i]; i++)
		if (s[i] == '\\')
			s[i] = '/';	
}

static void try_path(char *complete_path)
{
	static const char listext[12][12] = {".jpg", "\0"};
	if (fileLoader::is_valid(complete_path))
		return ;
	for (uint32_t i = 0; listext[i][0];i++)
	{
		strcat(complete_path, listext[i]);
		if (access(complete_path, R_OK))
		{
			return ;
		}
		complete_path[strlen(complete_path) - strlen(listext[i])] = 0;
	}
}

void 		renderDataSys::handle_texture(aiTextureType type, char *path, aiMaterial *material,
		uint32_t *textEmplacement, bool *has_text, uint32_t meshDataHandler)
{
	aiString					texturePath;
	char						completePath[1000];
	FREE_IMAGE_FORMAT			format;
	FIBITMAP*					image;
	GLuint 						textureID;
	uint32_t					instance;
	t_texture_request			request;

	memcpy(completePath, path, strlen(path) + 1);
	if (material->GetTextureCount(type) > 0)
	{
		material->GetTexture(type, 0, &texturePath);
		strcat(completePath, texturePath.C_Str());
		try_path(completePath);
		back_slash(completePath);
		textureID = fileLoader::load_fs_asset_sync(completePath, 1);
		*has_text = false;
		if (staticMemoryManager::get_asset_state(textureID) != staticMemoryManager::E_LOADED)
			return ;
		*has_text = true;
		instance = texture_builtin::create_instance(textureID);
		request.fieldID = 0;
		request.instanceHandler = instance;
		request.wrapt_filter = GL_REPEAT;
		request.wraps_filter = GL_REPEAT;
		request.min_filter = GL_LINEAR_MIPMAP_LINEAR;
		request.mag_filter = GL_LINEAR;
		request.meshDataHandler = meshDataHandler;
		renderDataSys::push_texture_request(request);
//		*textEmplacement = texture_builtin::convert_to_opengl(instance);
	}
}

void 		renderDataSys::copy_vertices(aiMesh *mesh, t_renderMeshData *meshData,
											const aiScene *scene, char *path, uint32_t meshHandler)
{
	float 						*vertices;
	GLuint						*indices;
	aiMaterial					*material;
	uint32_t					indices_size;
	uint32_t					vertex_size;
	aiColor4D 					color;
	uint32_t					vboHandler;
	uint32_t					indiceHandler;
	aiString					texturePath;
	char						completePath[1000];
	uint32_t					i = 0;
	t_vao_request 				request;

	memcpy(completePath, path, strlen(path) + 1);
	for (i = 0; completePath[strlen(completePath) - i] != '/'; i++)
		;
	completePath[strlen(completePath) - i + 1] = '\0';
	material = scene->mMaterials[mesh->mMaterialIndex];
	indices_size = mesh->mNumFaces;
	vertex_size = mesh->mNumVertices;
	indiceHandler = staticMemoryManager::create_asset(0, (indices_size * 3 * sizeof(uint32_t)));
	indices = (uint32_t*)staticMemoryManager::get_data_ptr(indiceHandler);
	for (uint32_t i = 0; i < indices_size; i++)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			GLuint indice = mesh->mFaces[i].mIndices[j];
			indices[i * 3 + j] = indice;
		}
	}
	vboHandler = staticMemoryManager::create_asset(0, ((vertex_size * sizeof(float) * 8)));
	vertices = (float*)staticMemoryManager::get_data_ptr(vboHandler);
	for (uint32_t i = 0; i < vertex_size; i++)
	{
		vertices[i * 8] = mesh->mVertices[i].x;
		vertices[i * 8 + 1] = mesh->mVertices[i].y;
		vertices[i * 8 + 2] = mesh->mVertices[i].z;
		vertices[i * 8 + 3] = mesh->mNormals[i].x;
		vertices[i * 8 + 4] = mesh->mNormals[i].y;
		vertices[i * 8 + 5] = mesh->mNormals[i].z;
		if (mesh->GetNumUVChannels() > 0) {
			vertices[i * 8 + 6] = mesh->mTextureCoords[0][i].x;
			vertices[i * 8 + 7] = mesh->mTextureCoords[0][i].y;
		}
	}
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
	meshData->material.diffuse[0] = color.r;
	meshData->material.diffuse[1] = color.g;
	meshData->material.diffuse[2] = color.b;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
	meshData->material.specular[0] = color.r;
	meshData->material.specular[1] = color.g;
	meshData->material.specular[2] = color.b;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color);
	meshData->material.ambiant[0] = color.r;
	meshData->material.ambiant[1] = color.g;
	meshData->material.ambiant[2] = color.b;
	//handle_texture(aiTextureType_NORMALS, completePath, material,
	//	&(meshData->material.bumpTexture), &(meshData->material.has_bump_texture), meshHandler);
	handle_texture(aiTextureType_DIFFUSE, completePath, material,
		&(meshData->material.diffuseTexture), &(meshData->material.has_diffuse_texture), meshHandler);
	//handle_texture(aiTextureType_SPECULAR, completePath, material,
	//	&(meshData->material.specularTexture), &(meshData->material.has_specular_texture), meshHandler);
	request.indices = indices;
	request.indices_size = indices_size * 3;
	request.vertex = vertices;
	request.vertex_size = vertex_size;
	request.meshDataHandler = meshHandler;
	push_request(request);
	meshData->indiceNum = indices_size * 3;
	meshData->indices = indiceHandler;
	meshData->vbo = vboHandler;
	meshData->vertexNum = vertex_size;
}

uint32_t 	renderDataSys::node_to_mesh(const aiNode *node,
				glm::mat4 trans, const aiScene *scene, char *path, uint32_t ref, uint32_t cluster)
{
	t_node 				*nodeData;
	t_renderMeshData	*mesh;
	uint32_t			k;
	uint32_t			meshHandler;

	nodeData = (t_node*)staticMemoryManager::get_data_ptr(ref);
	nodeData->meshs = staticMemoryManager::create_asset(cluster, sizeof(t_renderMeshData));
	nodeData->program = _programm[0];
	meshHandler = nodeData->meshs;
	for (int indexMesh = 0; indexMesh < node->mNumMeshes; indexMesh++)
	{
		nodeData->has_mesh = true;
		mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(meshHandler);
		copy_vertices(scene->mMeshes[node->mMeshes[indexMesh]], mesh, scene, path, meshHandler);
		mesh->has_child = true;
		mesh->child = staticMemoryManager::create_asset(cluster, sizeof(t_renderMeshData));
		meshHandler = mesh->child;
	}
	if (node->mNumMeshes)
	{
		mesh->has_child = false;
		nodeData->has_mesh = true;
	}
	else
		nodeData->has_mesh = false;
	for (k = 0; k < node->mNumChildren; k++)
	{
		nodeData->child[k] = staticMemoryManager::create_asset(cluster, sizeof(t_node));
		node_to_mesh(node->mChildren[k], trans, scene, path, nodeData->child[k], cluster);
	}
	nodeData->childNum = k;
}

void	renderDataSys::obj_scene_to_memory_as_mesh(const aiScene *scene, char *path, uint32_t ref, uint32_t cluster)
{
	glm::mat4 n;

	staticMemoryManager::alloc_asset(ref, sizeof(t_node));
	node_to_mesh(scene->mRootNode, n, scene, path, ref, cluster);
}

std::string 		readfile(std::string path)
{
	std::string line;
	std::string file;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while ( getline (myfile, line) )
			file += line + '\n';
		myfile.close();
	}
	else
	{
		std::cout << "Unable to open file";
		exit(0);
	}
	return file;
}

uint32_t renderDataSys::load_programVertexFrag(std::string vertexPath, std::string fragPath)
{
	GLchar			*vertexCode;
	GLchar			*fragCode;
	GLuint			vertexShaderObject;
	GLuint			fragmentShaderObject;
	GLint			flength, vlength;
	GLint			compiled;
	char			log[4096];
	GLint			slen, blen;
	std::string 	file;


	vertexCode = (GLchar*)malloc(409600);
	fragCode = (GLchar*)malloc(409600);
	file = readfile(vertexPath);
	memcpy(vertexCode, file.c_str(), file.length() + 1);
	file = readfile(fragPath);
	memcpy(fragCode, file.c_str(), file.length() + 1);
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	vlength = strlen(vertexCode);
	flength = strlen(fragCode);
	glShaderSource(vertexShaderObject, 1, (const GLchar **)&(vertexCode), &vlength);
	glCompileShader(vertexShaderObject);
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH , &blen);
		printf("%d\n", compiled);
		glGetShaderInfoLog(vertexShaderObject, blen, &slen, log);
		printf("VertexShader compilation: %s\n", log);
		exit(0);
	}
	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&(fragCode), &flength);
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH , &blen);
		glGetShaderInfoLog(fragmentShaderObject, blen, &slen, log);
		printf("Frag shder compilation: %s\n", log);
		exit(0);
	}
	GLint ProgramObject = glCreateProgram();
	glAttachShader(ProgramObject, vertexShaderObject);
	glAttachShader(ProgramObject, fragmentShaderObject);
	glLinkProgram(ProgramObject);
	free(vertexCode);
	free(fragCode);
	return (ProgramObject);
}


void renderDataSys::computeModification(t_renderMeshData *mesh, glm::vec3 translation, float angle, glm::vec3 axis)
{
	float *v;

	v = (float*)staticMemoryManager::get_data_ptr(mesh->vbo);
	for (uint32_t i = 0; i < mesh->vertexNum; i++)
	{

		v[i * 8] += translation.x;
		v[i * 8 + 1] += translation.y;
		v[i * 8 + 2] += translation.z;
	}
	mesh->vaoId = renderDataSys::createVAO();
	mesh->vboVerticeId = renderDataSys::createVBO_VNT(v, mesh->vertexNum, mesh->vaoId);
	mesh->indiceBufferId = renderDataSys::createVBO_Indice((uint32_t*)staticMemoryManager::get_data_ptr(mesh->indices),
		mesh->indiceNum, mesh->vaoId);
}

void renderDataSys::iterNode(t_node node, glm::vec3 translation, float angle, glm::vec3 axis)
{
	t_renderMeshData	*mesh;
	bool 				mesh_has_child;

	if (node.has_mesh)
	{
		mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(node.meshs);
		do
		{
			computeModification(mesh, translation, angle, axis);
			mesh_has_child = mesh->has_child;
			mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(mesh->child);
		}	while (mesh_has_child);
	}
	for (int i = 0; i < node.childNum; i++)
		iterNode(*(t_node*)(staticMemoryManager::get_data_ptr(node.child[i])), translation, angle, axis);

}

void renderDataSys::modifyVertices(uint32_t assetHandler, glm::vec3 translation, float angle, glm::vec3 axis)
{
	t_node *node;

	node = (t_node*)staticMemoryManager::get_data_ptr(assetHandler);
	iterNode(*node, translation, angle, axis);
}

void			renderDataSys::set_programm(E_SHADER shader, uint32_t assetHandler)
{
	t_node *node;

	node = (t_node*)staticMemoryManager::get_data_ptr(assetHandler);
	node->program = _programm[shader];
}