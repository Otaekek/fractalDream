#include "runtimeLibrary.class.hpp"
#include "../../headerList.hpp"

#ifdef  __APPLE__
# define FILE_AGE(F)(F.st_mtimespec.tv_sec)
#else
# define FILE_AGE(F)(F.st_mtim.tv_sec)
#endif

void runtimeLibrary::init()
{

}

void runtimeLibrary::shutdown()
{

}

void utoa(int n, char *s)
{
    int len = log10(n) + 1;
	int i = 0;

	if (!n)
	{
		s[0] = '0';
		s[1] = 0;
		return ;
	}
	s[len - i] = 0;
	while (n > 0)
	{
		s[len - i - 1] = n % 10 + '0';
		n /= 10;
		i++;
	}
}

void fill_sources(char ret[64][1024], char *sourceDir)
{
	DIR					*dir;
	uint32_t			i;
	struct dirent		*dirent;


	i = 0;
	ret[0][0] = 0;
	dir = opendir(sourceDir);
	if (dir == NULL)
	{
		printf("ERROR: cant open folder: %s\n", sourceDir);
		exit(0);//errorSys
	}
	while ((dirent = readdir(dir)))
	{

		if (dirent->d_type == DT_REG && !strcmp(dirent->d_name + strlen(dirent->d_name) - 4, ".cpp")
			&& strlen(dirent->d_name) + strlen(sourceDir) < 1022 && i < 64)
		{
			strncat(strcat(strcpy(ret[i], sourceDir), "/"), dirent->d_name, strlen(dirent->d_name) - 4);
			i++;
			ret[i][0] = 0;
		}
	}
	free(dir);
}

void *runtimeLibrary::create_lib(char *sourceDir)
{
	char sources[64][1024] = {0};

	fill_sources(sources, sourceDir);
	return (compile_object(sources, sourceDir));
}

bool create_stm_line(char *str, char *head, char *sources, struct stat libstat, int statret)
{
 	uint32_t 		i = 0;
 	struct stat 	filestat;
 	char 			pathstat[65536] = {0};

 	*str = 0;
	if (statret == 0 && stat(strcat(strcat(pathstat, sources), ".cpp"), &filestat) == 0)
	{
		if (FILE_AGE(filestat) < FILE_AGE(libstat))
			return (false);
 	}
	strcat(str, head);
	strcat(str, sources);
	strcat(str, ".o");
	strcat(str, " -c ");
	strcat(str, sources);
	strcat(str, ".cpp");
	return (true);
}

void link_library(char *stmline, char sources[][1024], char *srcdir)
{
	char head[] = "g++ -fPIC -Wl,-undefined -Wl,dynamic_lookup -shared ";

	strcat(stmline, head);
	strcat(stmline, " -o ");
	strcat(stmline, srcdir);
	strcat(stmline, "/");
	strcat(stmline, "0xdeadd00d.so ");
	for (uint32_t i = 0; sources[i][0]; i++)
	{
		strcat(stmline, sources[i]);
		strcat(stmline, ".o ");
	}
}

int	run_sys(char *str)
{
	int ret;

	write(1, "\x1B[34m", 6);
	write(1, str, strlen(str));
	write(1, "\n", 1);
	write(1, "\x1B[37m", 6);
	int pid = vfork();
	if (pid == 0)
		execl("/bin/sh", "sh","-c", str, (char*)0);
	while ((ret = waitpid(pid, NULL, 0)) > 0);
	return (0);
}

void *runtimeLibrary::compile_object(char sources[64][1024], char *srcdir)
{
	char 		stmLineHead[65536] = "g++ -std=c++11 -fPIC ";
	char 		stmLine[65536] = {0};
	void 		*handle;

	struct stat 		libstat;
	int 				statret;

	statret = stat(strcat(strcat(strcat(stmLine, srcdir),  "/"), "0xdeadd00d.so"), &libstat);
	stmLine[0] = 0;
	strcat(stmLineHead, headerList);
	strcat(stmLineHead, " -o ");
	uint32_t	i = 0;
	void (*f)(void);
	while (sources[i][0])
	{
		if (create_stm_line(stmLine, stmLineHead, sources[i], libstat, statret) == false)
		{
			i++;
			continue ;
		}
		if (run_sys(stmLine) < 0)
		{
			printf("Script cannot be compiled\n");
			return NULL;
		}
		i++;
	}
	stmLine[0] = 0;
	link_library(stmLine, sources, srcdir);
	if (run_sys(stmLine) < 0)
	{
		printf("Script cannot be compiled\n");
		return NULL;
	}
	*stmLine = 0;
	strcat(stmLine, srcdir);
	strcat(stmLine, "/");
	strcat(stmLine,  "0xdeadd00d.so");
	handle = link(stmLine);
	if (!handle)
	{
		printf("%s\n", dlerror());
		return (NULL);
	}
	f = (void(*)())dlsym(handle, "_Z5debutv");
	if (f)
		f();
	return (handle);
}

void *runtimeLibrary::link(char *sourcesLib)
{
	return (dlopen(sourcesLib, RTLD_NOW | RTLD_GLOBAL));
}

void runtimeLibrary::close_lib(void *handle)
{
	dlclose(handle);	
}
