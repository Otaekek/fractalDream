#include "directorySmartLoader.class.hpp"


void	travel(char *path)
{
	DIR 			*dir;
	struct dirent 	*dirent;

	dir = opendir(path);
	if (dir == NULL)
	{
		printf("%s %s %s %s\n", "Error in", __FILE__, ": Unable to open directory:", path);
		return ;
	}
	while ((dirent) = readdir(dir))
	{
		char *pathdir;

		pathdir = (char*)calloc(40960, 1);
		strcat(strcat(strcat(pathdir, path), "/"), dirent->d_name);
		if (dirent->d_type == DT_DIR && dirent->d_name[0] != '.' && dirent->d_name[1] != '.')
			travel(pathdir);
		if (fileLoader::is_loadable_as_3d_asset(pathdir))
			mapEditorBuiltin::push_elem(fileLoader::load_fs_asset_async(pathdir, (uint32_t)E_3D));
		free(pathdir);
	}
	free(dir);
}

void 	directorySmartLoader::execute()
{
	char path[4096] = {0};

	strcpy(path, "./assets/jojishiAssets");
	travel(path);
}