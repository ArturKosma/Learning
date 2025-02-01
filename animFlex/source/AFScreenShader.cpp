#include "AFScreenShader.h"

#include "AFUtility.h"

void AFScreenShader::UniformBinding()
{
	glUseProgram(m_shaderProgram);

	GLint texLocation = glGetUniformLocation(m_shaderProgram, "Tex");

	glUniform1i(texLocation, 0);

	glUseProgram(0);
}
