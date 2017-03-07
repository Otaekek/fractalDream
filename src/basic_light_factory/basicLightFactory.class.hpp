#ifndef BASICLIGHTFACTORY_CLASS_HPP
#define BASICLIGHTFACTORY_CLASS_HPP

#include <transform.class.hpp>
#include <renderDataSys.class.hpp>
#include <renderBuiltIn.class.hpp>

#define LIGHT_FACTORY_MAX_ELEM 4096

class basicLight {

public:

	basicLight();
	basicLight(glm::vec3 position, glm::vec3 direction, glm::vec3 scale, E_LIGHT type, glm::vec3 color);
	~basicLight();

	void update();

private:

	uint32_t	transformHandler;
	uint32_t	lightHandler;
	uint32_t	renderGoHandler;

};

class basicLightFactory {

public:

	static void		init();
	static void		shutdown();

	static void		create(glm::vec3 position, glm::vec3 direction, glm::vec3 scale, E_LIGHT type, glm::vec3 color);

	static void		update();

private:

	static uint32_t		numElem;
	static basicLight	elems[LIGHT_FACTORY_MAX_ELEM];
};

#endif