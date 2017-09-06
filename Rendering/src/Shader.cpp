#include "Shader.h"
#include <gl_core_4_4.h>
#include <ReadFile.h>


Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::bind()
{
}

void Shader::unbind()
{
}

void Shader::load(const char* filename, unsigned type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		vsSource = FileRead(filename);
		m_vertexShader = glCreateShader(type);
		glShaderSource(m_vertexShader, 1, (const char **)&vsSource, 0);
		glCompileShader(m_vertexShader);
		break;
	case GL_FRAGMENT_SHADER:
		fsSource = FileRead(filename);
		m_fragmentShader = glCreateShader(type);
		glShaderSource(m_fragmentShader, 1, (const char **)&fsSource, 0);
		glCompileShader(m_fragmentShader);
		break;
	}
}

void Shader::attach()
{
}

void Shader::defaultLoad()
{
}

unsigned Shader::getUniform(const char*)
{
	return 0;
}
