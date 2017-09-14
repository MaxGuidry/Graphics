#pragma once
class Shader
{
public:
	Shader();
	Shader(unsigned int programID);
	~Shader();
	void bind();
	void unbind();
	void load(const char * filename, unsigned int type);
	void attach();
	void defaultLoad();
	//void load();
	unsigned int getUniform(const char *);
	unsigned int m_Shader;
	const char* Source;
	

private:
	unsigned int m_program;
};