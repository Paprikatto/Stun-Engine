#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const int width = 640;
	const int height = 480;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew error" << '\n';

    std::cout << glGetString(GL_VERSION) << '\n';

	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f, 1.0f, 0.0f,
			50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f, 0.0f, 1.0f
		};

    	unsigned int indices[] = {
    		0, 1, 2,
			2, 3, 0
		};

    	GL_CALL(glEnable(GL_BLEND));
    	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    	
    	//vertex array stores the vertex buffers paired with the layout
    	VertexArray va;
    	//vertex buffer stores the vertex data
    	VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    	
		VertexBufferLayout layout;
    	//2 floats per vertex (x and y)
    	layout.Push<float>(2);
    	//texture coordinates
    	layout.Push<float>(2);
    	va.AddBuffer(vb, layout);
    	
    	IndexBuffer ib(indices, 6);

    	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 200.0f, 0.0f));

    	glm::mat4 mvp = proj * view * model;
    	
		Shader shader = Shader("res/shaders/Basic.shader");
    	shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    	Texture texture("res/textures/tex.jpg");
    	texture.Bind();
    	shader.SetUniform1i("u_Texture", 0);
    	shader.SetUniformMat4f("u_MVP", mvp);

		va.Unbind();
    	shader.Unbind();
    	vb.Unbind();
    	ib.Unbind();
    	Renderer renderer;

    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();
    	ImGuiIO& io = ImGui::GetIO();
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    	ImGui_ImplGlfw_InitForOpenGL(window, true);
    	ImGui_ImplOpenGL3_Init();
    	ImGui::StyleColorsDark();
    	
    	
    	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    	glm::vec3 translation2 = glm::vec3(200.0f, 0.0f, 0.0f);
    	/* Loop until the user closes the window */
    	while (!glfwWindowShouldClose(window))
    	{
    		renderer.Clear();

    		//imgui initialization
    		ImGui_ImplOpenGL3_NewFrame();
    		ImGui_ImplGlfw_NewFrame();
    		ImGui::NewFrame();

    		shader.Bind();
    		{
    			model = glm::translate(glm::mat4(1.0f), translation);
    			mvp = proj * view * model;
    			shader.SetUniformMat4f("u_MVP", mvp);
    			renderer.Draw(va, ib, shader);
    		}
    		{
    			model = glm::translate(glm::mat4(1.0f), translation2);
    			mvp = proj * view * model;
    			shader.SetUniformMat4f("u_MVP", mvp);
    			renderer.Draw(va, ib, shader);
    		}
    		//imgui
    		{
    			ImGui::SliderFloat3("position", &translation.x, 0.0f, 200.0f);
    			ImGui::SliderFloat3("position2", &translation2.x, 0.0f, 200.0f);
    		}
    		//imgui end
    		ImGui::Render();
    		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    		
    		/* Swap front and back buffers */
    		glfwSwapBuffers(window);

    		/* Poll for and process events */
    		glfwPollEvents();
    		
    	}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
    glfwTerminate();
    return 0;
}