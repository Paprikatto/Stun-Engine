#include "Shader.h"
#include  "Renderer.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

Shader::Shader(const std::string& filepath): m_RendererID(0), m_Filepath(filepath)
{
	std::string vs, fs;
    ParseShader(vs, fs);
	m_RendererID = CreateShaders(vs, fs);
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::string vs, fs;
	LoadFile(vertexShader, vs);
	LoadFile(fragmentShader, fs);
	m_RendererID = CreateShaders(vs, fs);
}

Shader::Shader(const unsigned char* vertexShader, unsigned int vertexShaderSize, const unsigned char* fragmentShader,
	unsigned int fragmentShaderSize)
{
	//TODO: check if unsigned int value is bigger than int max value
	const char *vertexData = reinterpret_cast<const char*>(vertexShader);
	const char *fragmentData = reinterpret_cast<const char*>(fragmentShader);
	m_RendererID = CreateShaders(vertexData, static_cast<int>(vertexShaderSize),
		fragmentData, static_cast<int>(fragmentShaderSize));
	
}

Shader::~Shader()
{
	GL_CALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GL_CALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GL_CALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GL_CALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GL_CALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetVec3f(const std::string& name, float v0, float v1, float v2)
{
	int loc = GetUniformLocation(name);
	GL_CALL(glUniform3f(loc, v0, v1, v2));
}

void Shader::SetVec3f(const std::string& name, const glm::vec3& vec)
{
	int loc = GetUniformLocation(name);
	GL_CALL(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void Shader::SetVec4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int loc = GetUniformLocation(name);
	GL_CALL(glUniform4f(loc, v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	GL_CALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	GL_CALL(int loc = glGetUniformLocation(m_RendererID, name.c_str()));
	if (loc == -1)
	{
		std::cout << "Error getting uniform location " << name << std::endl;
	}
	return loc;
}

void Shader::ParseShader(std::string& vertexShader, std::string& fragmentShader) {
    ShaderType type = ShaderType::NONE;

    std::ifstream stream(m_Filepath);
    
    std::stringstream ss[2];
    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
                
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
        }
        else{
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    vertexShader = ss[0].str();
	fragmentShader = ss[1].str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    }

	return id;
}

unsigned int Shader::CompileShader(unsigned int type, const char* data, const int length)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &data, &length);
	glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    }

	return id;
}

unsigned int Shader::CreateShaders(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs); 

	return program;
}

unsigned int Shader::CreateShaders(const char* vertexShaderData, int vertexShaderLength, const char* fragmentShaderData,
	int fragmentShaderLength)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderData, vertexShaderLength);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderData, fragmentShaderLength);

    glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs); 

	return program;
}

void Shader::LoadFile(const std::string& filepath, std::string& target)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << filepath << std::endl;
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	target = buffer.str();
}
