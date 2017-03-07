#ifndef TERRAINGENERATIONBUILTIN_CLASS_HPP
#define TERRAINGENERATIONBUILTIN_CLASS_HPP

#include <staticMemoryManager.class.hpp>
#include <renderDataSys.class.hpp>
#include <renderBuiltIn.class.hpp>
#include <transform.class.hpp>

#define FRAGMENT_SIZE 100

typedef struct	s_biom {

	float				size;
	float				posx;
	float				posy;
	uint32_t			numFrag;
	uint32_t			albedoTextureInstance;
	uint32_t			heightTextureInstance;
	uint32_t			transformHandler;
	uint32_t			renderGoHandler;
}						t_biom;

class terrainGenerationBuiltin {


public:

	static void 		init();

	static void 		update();

	static void			add_biom(float posx, float posy, float posz, uint32_t size,
					float scale, float ampl, uint32_t textureInstance, float textScale, uint32_t heitmapTextureInstance);

private:

	static t_biom		bioms[4096];
	static uint32_t		numBiom;

};

#endif