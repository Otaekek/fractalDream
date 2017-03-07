#ifndef ENTITY_CLASS_HPP
#define ENTITY_CLASS_HPP

#define MAX_COMPONENT 12

enum e_qtype {
	E_SCRIPT,
	E_RENDER,
	E_CAMERA,
	E_TERRAIN,
	E_LIGHT,
	E_SIZEQ
};

class entity {

public:

	entity();
	~entity();

	void		set_index(uint32_t index) {_manager_index = index;}
	uint32_t	get_index() {return _manager_index;}
	void		update();

	void del(uint32_t q, uint32_t handler);
	void push(uint32_t q, uint32_t handler);

private:

	uint32_t	_queues[E_SIZEQ][MAX_COMPONENT];
	uint32_t	_counts[E_SIZEQ];

	uint32_t	_manager_index;
	uint32_t	_transform;
}
#endif 