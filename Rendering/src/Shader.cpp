#include "Shader.h"
#include <gl_core_4_4.h>
#include <ReadFile.h>
#include <cstdio>


Shader::Shader()
{
}

Shader::Shader(unsigned programID)
{
	m_program = programID;
}


Shader::~Shader()
{
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::load(const char* filename, unsigned type)
{
	Source = FileRead(filename);
	m_Shader = glCreateShader(type);
	glShaderSource(m_Shader, 1, (const char **)&Source, 0);
	glCompileShader(m_Shader);

}

void Shader::attach()
{
	int success = GL_FALSE;
	glAttachShader(m_program, m_Shader);
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		const char * test = Source;
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//glDeleteShader(m_Shader);
}

void Shader::defaultLoad()
{
}

unsigned Shader::getUniform(const char* name)
{
	return 0;
}
