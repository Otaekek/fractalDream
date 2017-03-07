#ifndef RUNTIME_LIBRARY_CLASS_HPP
#define RUNTIME_LIBRARY_CLASS_HPP

#include <cstdlib>
#include <math.h> 
#include <cstring>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>

class runtimeLibrary {

public:

	static void init();
	static void shutdown();

	static void *create_lib(char *sourcesPath);
	static void close_lib(void *handle);

private:

	static void *compile_object(char sources[][1024], char *_);
	static void *link(char *sourceDir);

};

#endif