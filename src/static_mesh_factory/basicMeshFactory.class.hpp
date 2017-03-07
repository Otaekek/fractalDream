#ifndef BASICMESHFACTORY
#define BASICMESHFACTORY

#include <transform.class.hpp>
#include <renderBuiltIn.class.hpp>
#include <fileLoader.class.hpp>
#include <staticMemoryManager.class.hpp>

#define MESH_FACTORY_MAX_ELEM 4096

class staticMesh {

public:

	staticMesh();
	staticMesh(glm::vec3 pos, glm::vec3 rot, float angle, glm::vec3 scale, std::string meshName);
	staticMesh(uint32_t meshHandler, uint32_t _transformHandler);
	~staticMesh();

	void update();

private:

	uint32_t	transformHandler;
	uint32_t	assetHandler;
	uint32_t	renderGOhandler;

};

class staticMeshManager {

public:

	static void		init();
	static void		shutdown();

	static void		create(glm::vec3 pos, glm::vec3 rot, float angle, glm::vec3 scale, std::string meshName);
	static void		create_from_handler(uint32_t meshHandler, uint32_t transformHandler);
	static void		update();

private:

	static uint32_t		numElem;
	static staticMesh	elems[MESH_FACTORY_MAX_ELEM];
};

#endif