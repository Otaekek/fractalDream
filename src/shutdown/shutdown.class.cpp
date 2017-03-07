#include  "shutdown.class.hpp"

void shutdown::exit_jojishi(void)
{
	staticMemoryManager::shutdown();
	jobHandler::shutdown();
	texture_builtin::shutdown();
	renderBuiltIn::shutdown();
	exit(0);
}