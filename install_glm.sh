wget  https://github.com/g-truc/glm/releases/download/0.9.8.3/glm-0.9.8.3.zip
unzip glm-0.9.8.3.zip
cd glm
cmake .
make 
cd ..
rm  glm-0.9.8.3.zip
echo "GLM successfully installed."
