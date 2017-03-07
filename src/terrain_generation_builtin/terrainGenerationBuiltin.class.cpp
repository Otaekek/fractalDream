#include "terrainGenerationBuiltin.class.hpp"
#include <ctime>
t_biom		terrainGenerationBuiltin::bioms[4096] = {0};
uint32_t	terrainGenerationBuiltin::numBiom = 0;


void		terrainGenerationBuiltin::init()
{

}

void		terrainGenerationBuiltin::update()
{
	for (uint32_t i = 0; i < numBiom; i++)
			renderBuiltIn::render_me(bioms[i].renderGoHandler);
}

t_material	handle_material(uint32_t albedoTextureInstance)
{
	t_material material = {0};

	material.diffuse[0] = 1;
	material.ambiant[0] = 0;
	material.ambiant[1] = 0;
	material.ambiant[2] = 0;
	if (albedoTextureInstance != 0)
	{
		material.has_diffuse_texture = 1;
		material.diffuseTexture = texture_builtin::convert_to_opengl_parametric(albedoTextureInstance, GL_REPEAT,
			GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	}
	return (material);
}

void	create_normal(float *data, uint32_t i, uint32_t j, uint32_t size, float *outvec, float divisor, float ampl, float scale)
{
	glm::vec3 base = glm::vec3(0, -0.01 / ampl, 0);

	float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
	int32_t i1, i2,i3,i4,i5,i6,i7,i8;

	i1 = (j - 1) + (i + 1) * size;
	i2 = (j + 0) + (i + 1) * size;
	i3 = (j + 1) + (i + 1) * size;
	i4 = (j - 1) + (i + 0) * size;
	i5 = (j + 1) + (i + 0) * size;
	i6 = (j - 1) + (i - 1) * size;
	i7 = (j + 0) + (i - 1) * size;
	i8 = (j + 1) + (i - 1) * size;

	if (i1 > 0)
		a = data[i1];
	if (i2 > 0)
		b = data[i2];
	if (i3 > 0)
		c = data[i3];
	if (i4 > 0)
		d = data[i4];
	if (i5 > 0)
		e = data[i5];
	if (i6 > 0)
		f = data[i6];
	if (i7 > 0)
		g = data[i7];
	if (i8 > 0)
		h = data[i8];

	base.z = (d - e);
	base.x = (g - b);
	base.x = -(c - a + 2.0f * (e - d) + h - f) / (size * scale);
	base.z = -(f - a + 2.0f * (g - b) + h - c) / (size * scale);
	base = glm::normalize(base);
	outvec[0] = -base.x;
	outvec[1] = -base.y;
	outvec[2] = -base.z;
}

void	create_vbo(int32_t size, float *data, t_renderMeshData *meshData, float scale, float ampl, uint32_t albedoTextureInstance, float textScale)
{
	float		*vertex;
	uint32_t	*indices;
	uint32_t	k;

	k = 0;
	vertex = (float*)malloc(sizeof(float) * size * size * 8);
	indices = (uint32_t*)malloc(sizeof(uint32_t) * size * size * 6);
	float treshOld = size * 0.1;
	float divisor;
	meshData->vaoId = renderDataSys::createVAO();

	meshData->vertexNum = size * size;
	meshData->indiceNum = size * size;
	meshData->material = handle_material(albedoTextureInstance);
	bzero(indices, size * size * sizeof(uint32_t) * 6);

	for (int32_t i = 0; i < size; i++)
	{
		for (int32_t j = 0; j < size; j++)
		{
			divisor = 1;
			// Set position

			vertex[i * size * 8 + j * 8] = (float)(i - size / 2) * scale;
			if (size - j < treshOld)
					divisor = (float)(size - j) / treshOld;
			if (j < treshOld)
				divisor = fmin(divisor, (float)(j) / treshOld);
			if (size - i < treshOld)
					divisor = fmin(divisor, (float)(size - i) / treshOld);
			if (i < treshOld)
				divisor = fmin(divisor, (float)(i) / treshOld);
			if (divisor < 0.1)
				divisor = 0;

			vertex[i * size * 8 + j * 8 + 1] = data[j + i * size] * ampl * divisor;
			vertex[i * size * 8 + j * 8 + 2] = (float)(j - size / 2) * scale;

			// Set normal
			create_normal(data, i, j, size, &vertex[i * size * 8 + j * 8 + 3], divisor, ampl, scale);

			// Set UV
			vertex[i * size * 8 + j * 8 + 6] = (float)i / size * textScale;
			vertex[i * size * 8 + j * 8 + 7] = (float)j / size * textScale;

			// Set indices
			if (j != size - 1 && i != 0 && j != 0 && i != size - 1)
			{
				indices[i * size * 6 + j * 6] = k;
				indices[i * size * 6 + j * 6 + 1] = k + 1;
				indices[i * size * 6 + j * 6 + 2] = k + size;

				indices[i * size * 6 + j * 6 + 3] = k;
				indices[i * size * 6 + j * 6 + 4] = k - 1 + size;
				indices[i * size * 6 + j * 6 + 5] = k + size;
			}
			k++;
		}
	}
	meshData->vboVerticeId = renderDataSys::createVBO_VNT(vertex, size * size, meshData->vaoId);
	meshData->indiceBufferId = renderDataSys::createVBO_Indice(indices, size * size * 6, meshData->vaoId);
	meshData->indiceNum = size * 6 * size;
	free(indices);
	free(vertex);
}

void update_object(uint32_t size, float *data, t_renderMeshData *meshData, float scale, float ampl, uint32_t albedoTextureInstance, float textScale)
{
	meshData->has_child = false;
	meshData->child = 0;
	create_vbo(size, data, meshData, scale, ampl, albedoTextureInstance, textScale);
}

float		*fill_data(uint32_t textureInstance, uint32_t mapSize, float ampl)
{
	t_textureInstance	*instance;
	t_texture			*texture;
	unsigned char				*data;
	float 				*ret;
	uint32_t			textSize;
	float				ratio;

	instance = (t_textureInstance*)dynamicMemoryManager::get_ptr(textureInstance);
	texture = (t_texture*)staticMemoryManager::get_data_ptr(instance->textureHandler);
	data = (unsigned char*)staticMemoryManager::get_data_ptr(texture->textureData);
	ret = (float*)malloc(mapSize * mapSize * sizeof(float)* 10);
	textSize = std::min(texture->sizex, texture->sizey);
	ratio = (float)mapSize / textSize;
	for (uint32_t i = 0; i < mapSize * mapSize; i++)
	{
		uint32_t index = i / (int)((float)mapSize * ratio) / ratio * mapSize + (i % mapSize) / ratio;
		index *= 4;
		ret[i] = (uint64_t)(data[index + 0]) << 16;
		ret[i] += (uint64_t)data[index + 1] << 8;
		ret[i] += data[index + 2];
		ret[i] /= (UINT32_MAX /1000);
	}
	return (ret);
}

void		terrainGenerationBuiltin::add_biom(float posx, float posy, float posz, uint32_t size,
					float scale, float ampl, uint32_t textureInstance, float textScale, uint32_t heitmapTextureInstance)
{
	t_biom				biom;
	uint32_t			i;
	uint32_t			meshDataHandler;
	t_node				*node;
	float 				*data;

	size += 3;
	biom.posy = posy;
	biom.posx = posx;
	biom.size = size;
	biom.albedoTextureInstance = textureInstance;
	biom.heightTextureInstance = heitmapTextureInstance;
	data = fill_data(heitmapTextureInstance, size, ampl);
	meshDataHandler = staticMemoryManager::create_asset(1, sizeof(t_node));
	node = (t_node*)staticMemoryManager::get_data_ptr(meshDataHandler);
	node->has_mesh = 1;
	node->childNum = 0;
	node->meshs = staticMemoryManager::create_asset(0, sizeof(t_renderMeshData));
	renderDataSys::set_programm(E_DEFAULT, meshDataHandler);
	biom.renderGoHandler = renderBuiltIn::create();
	(renderBuiltIn::get_renderGO(biom.renderGoHandler))->assetHandler = meshDataHandler;
	(renderBuiltIn::get_renderGO(biom.renderGoHandler))->transformHandler = transformBuiltin::create();
	transformBuiltin::translate((renderBuiltIn::get_renderGO(biom.renderGoHandler))->transformHandler, posx, posy, posz);
	update_object(size, data, (t_renderMeshData*)staticMemoryManager::get_data_ptr(node->meshs), scale, ampl, textureInstance, textScale);
	biom.numFrag = i;
	bioms[numBiom++] = biom;
	free(data);
}
