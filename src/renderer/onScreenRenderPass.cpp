#include <renderBuiltIn.class.hpp>

void renderBuiltIn::renderOnScreen()
{
	GLuint texID;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mode->width, mode->height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderDataSys::_programm[E_ONSCREEN]);
	glBindVertexArray(_quadVertexbuffer);
	glBindTexture(GL_TEXTURE_2D, _colorTextureid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderBuiltIn::render_fractal(t_camera *camera)
{
	GLuint texID;
	GLuint location;

	glViewport(0, 0, mode->width, mode->height);
	glUseProgram(renderDataSys::_programm[E_TESTFRACTAL]);
	glBindVertexArray(_quadVertexbuffer);
	location = glGetUniformLocation(renderDataSys::_programm[E_TESTFRACTAL], "resolution");
	glUniform2f(location, mode->width, mode->height);
	//glClear(GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 viewMat = transformBuiltin::to_mat(camera->transformHandler);
	GLint model = glGetUniformLocation(renderDataSys::_programm[E_TESTFRACTAL], "view");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(viewMat));

	GLint time = glGetUniformLocation(renderDataSys::_programm[E_TESTFRACTAL], "time");
	glUniform1f(time, (float)clock());

	glDrawArrays(GL_TRIANGLES, 0, 6);
}