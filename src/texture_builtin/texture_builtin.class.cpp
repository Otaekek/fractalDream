
#include "texture_builtin.class.hpp"

uint32_t	texture_builtin::cluster_id;
FIBITMAP*	texture_builtin::garb_collector[65536];
uint32_t	texture_builtin::numGarb = 0;


uint32_t	texture_builtin::init()
{
	cluster_id = dynamicMemoryManager::cluster_init(sizeof(t_textureInstance), 4096);
}

void	texture_builtin::shutdown()
{
	while (numGarb > 0)
	{
		FreeImage_Unload(garb_collector[numGarb]);
		numGarb--;
	}
}

char 		*texture_builtin::get_data_from_instance(uint32_t textureInstance)
{
	t_textureInstance *tx;

	tx = get_texture(textureInstance);
	return (char*)(staticMemoryManager::get_data_ptr(((t_texture*)(staticMemoryManager::get_data_ptr(tx->textureHandler)))->textureData));
}

void		texture_builtin::load_texture(void *data)
{
	FREE_IMAGE_FORMAT		format;
	FIBITMAP*				image, *tmp;
	t_loadHeader 			*loadheader;
	t_texture				*texture;
	char					*textureData;

	loadheader = (t_loadHeader*)data;
	format = FreeImage_GetFileType(loadheader->path, 0);
	tmp = FreeImage_Load(format, loadheader->path);
	image = FreeImage_ConvertTo32Bits(tmp);
	FreeImage_Unload(tmp);
	garb_collector[++numGarb] = image;
	staticMemoryManager::alloc_asset(loadheader->ref, sizeof(t_texture));
	texture = (t_texture*)staticMemoryManager::get_data_ptr(loadheader->ref);
	texture->sizex = FreeImage_GetWidth(image);
	texture->sizey = FreeImage_GetHeight(image);
	texture->textureData = staticMemoryManager::create_asset(loadheader->cluster, texture->sizex * texture->sizey * 4);
	textureData = (char*)staticMemoryManager::get_data_ptr(texture->textureData);
	memcpy(textureData, FreeImage_GetBits(image), texture->sizex * texture->sizey * 4);
	staticMemoryManager::set_asset_state(staticMemoryManager::E_LOADED, loadheader->ref);
}

uint32_t 	texture_builtin::create_instance(uint32_t ref)
{
	t_textureInstance *texture;
	uint32_t			instanceRef;

	instanceRef = dynamicMemoryManager::create_slot(cluster_id);
	texture = get_texture(instanceRef);
	texture->textureHandler = ref;
	return (instanceRef);
}

uint32_t 	texture_builtin::destroy_instance(uint32_t ref)
{
	dynamicMemoryManager::clear_data(ref, cluster_id);
}

t_textureInstance 	*texture_builtin::get_texture(uint32_t textureHandler)
{
	return (t_textureInstance*)dynamicMemoryManager::get_ptr(textureHandler);
}

GLuint	texture_builtin::convert_to_opengl(uint32_t textureInstanceHandler)
{
	GLuint		textureID;
	t_texture	*texture;
	t_textureInstance *tx;

	tx = get_texture(textureInstanceHandler);
	texture = (t_texture*)staticMemoryManager::get_data_ptr(tx->textureHandler);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->sizex, texture->sizey, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, (char*)staticMemoryManager::get_data_ptr(texture->textureData));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (textureID);
}


GLuint	texture_builtin::convert_to_opengl_parametric(uint32_t textureInstanceHandler, uint32_t wraps, uint32_t wrapt, uint32_t min, uint32_t mag)
{
	GLuint		textureID;
	t_texture	*texture;
	t_textureInstance *tx;

	tx = get_texture(textureInstanceHandler);
	texture = (t_texture*)staticMemoryManager::get_data_ptr(tx->textureHandler);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->sizex, texture->sizey, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, (char*)staticMemoryManager::get_data_ptr(texture->textureData));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (textureID);
}
