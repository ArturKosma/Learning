#include "AFPostprocessShader.h"

void AFPostprocessShader::UniformBinding()
{
    AFShader::UniformBinding();

    glUseProgram(m_shaderProgram);

    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "u_ColorTex");
    GLint depthLoc = glGetUniformLocation(m_shaderProgram, "u_DepthTex");
    GLint stencilLoc = glGetUniformLocation(m_shaderProgram, "u_StencilTex");

    if (colorLoc != -1)
    {
        glUniform1i(colorLoc, 0);
    }

    if(depthLoc != -1)
    {
        glUniform1i(depthLoc, 1);
    }

    if (stencilLoc != -1)
    {
        glUniform1i(stencilLoc, 2);
    }

    glUseProgram(0);
}
