#ifndef TEXTURE_BUILTIN_CLASS_HPP
#define TEXTURE_BUILTIN_CLASS_HPP

#include <staticMemoryManager.class.hpp>
#include <renderDataSys.class.hpp>
#include <dynamicMemoryManager.class.hpp>
#include <FreeImage.h>

typedef struct	s_texture {
	uint32_t	sizex;
	uint32_t	sizey;
	uint32_t	textureData;
}				t_texture;

typedef struct	s_textureInstance {
	uint32_t	textureHandler;
}				t_textureInstance;

class texture_builtin {

public :
	
	static void					shutdown();
	static uint32_t				init();
	static void					load_texture(void *data);
	static uint32_t 			create_instance(uint32_t ref);
	static uint32_t 			destroy_instance(uint32_t ref);
	static t_textureInstance 	*get_texture(uint32_t textureHandler);
	static GLuint				convert_to_opengl(uint32_t textureInstanceHandler);
	static char					*get_data_from_instance(uint32_t textureInstance);
	static GLuint				convert_to_opengl_parametric(uint32_t textureInstanceHandler, uint32_t wraps,
									uint32_t wrapt, uint32_t min, uint32_t mag);

private :

	static FIBITMAP*	garb_collector[65536];
	static uint32_t		numGarb;
	static uint32_t		cluster_id;

};
#endif

