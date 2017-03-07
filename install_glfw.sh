wget https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
unzip glfw-3.2.1.zip
cd glfw-3.2.1
cmake .
make
sudo make install
cd ..
rm glfw-3.2.1.zip
