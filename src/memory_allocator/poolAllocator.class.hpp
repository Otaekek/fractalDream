#ifndef POOLALLOCATOR_CLASS_HPP
#define POOLALLOCATOR_CLASS_HPP

#include <cstdint>

class poolAllocator {

public:

	typedef struct 			s_bloc {
		char 				*mem;
		struct s_bloc		*next_elem;
		uint32_t			size;
	}						t_bloc;

	poolAllocator();
	poolAllocator(uint32_t bloc_size, uint32_t bloc_num);
	~poolAllocator();

	void 						modifySize(uint32_t bloc_size, uint32_t bloc_num);
	void						*mem_alloc(void);
	void 						mem_free(void *bloc);

private:

	char 		*data;
	t_bloc 		*free_list;
	t_bloc		*free_list_last;
};

#endif