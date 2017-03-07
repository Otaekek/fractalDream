export LD_LIBRARY_PATH=/home/otae/Projects/Jojishi/assimp/lib
git clone git://github.com/assimp/assimp.git assimp
cd assimp
cmake CMakeLists.txt -G 'Unix Makefiles'
make

