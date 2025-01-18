#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

static void ParseShader(const std::string& filepath, std::string& vertexShader, std::string& fragmentShader) {
    enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

    ShaderType type = ShaderType::NONE;

    std::ifstream stream(filepath);
    
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

static unsigned int CompileShader(unsigned int type, const std::string& source) {
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

static unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader) {
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
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			 -0.5f, 0.5f,
		};

    	unsigned int indices[] = {
    		0, 1, 2,
			2, 3, 0
		};

    	VertexArray va;
    	VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    	
		VertexBufferLayout layout;
    	//2 floats per vertex (x and y)
    	layout.Push<float>(2);
    	va.AddBuffer(vb, layout);
    	
    	IndexBuffer ib(indices, 6);

    	std::string vertexShader;
    	std::string fragmentShader;

    	ParseShader("./res/shaders/Basic.shader", vertexShader, fragmentShader);

    	unsigned int shader = CreateShaders(vertexShader, fragmentShader);
    	glUseProgram(shader);

    	int location = glGetUniformLocation(shader, "u_Color");
    	ASSERT(location != -1);
    	glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

    	glBindVertexArray(0);
    	glUseProgram(0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    	/* Loop until the user closes the window */
    	while (!glfwWindowShouldClose(window))
    	{
    		/* Render here */
    		glClear(GL_COLOR_BUFFER_BIT);

    		glUseProgram(shader);
    		glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

    		va.Bind();
    		ib.Bind();
    		GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    		/* Swap front and back buffers */
    		glfwSwapBuffers(window);

    		/* Poll for and process events */
    		glfwPollEvents();
    	}
    	GL_CALL(glDeleteProgram(shader));
	}
    glfwTerminate();
    return 0;
}