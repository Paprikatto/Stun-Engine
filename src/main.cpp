#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
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
			//position         //normal
			//front
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			//back
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			//left
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			//right
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			//bottom
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			//top
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		};

    	unsigned int indices[] = {
    		// Front face
    		0, 1, 2,
			2, 3, 0,
    		// Back face
			4, 6, 5,
			6, 4, 7,
			// Left face
			8, 9, 10,
			10, 11, 8,
			// Right face
			12, 14, 13,
			14, 12, 15,
			// Bottom face
			16, 18, 17,
			18, 16, 19,
			// Top face
			20, 21, 22,
			22, 23, 20
		};

    	GL_CALL(glEnable(GL_BLEND));
    	glEnable(GL_DEPTH_TEST);
    	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    	//vertex array stores the vertex buffers paired with the layout
    	VertexArray va;
    	//vertex buffer stores the vertex data
    	VertexBuffer vb(positions, 6 * 4 * 6 * sizeof(float));

		VertexBufferLayout layout;
    	//3 floats per vertex (x, y, z)
    	layout.Push<float>(3);
    	//color
    	layout.Push<float>(3);
    	va.AddBuffer(vb, layout);

    	IndexBuffer ib(indices, 3 * 12);

    	glm::mat4 proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 200.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f));
    	glm::mat4 model = glm::mat4(1.0f);

    	Shader light_shader = Shader("res/shaders/LightVertex.glsl", "res/shaders/LightFragment.glsl");
    	light_shader.Bind();
    	light_shader.SetVec3f("objectColor", 0.6f, 0.6f, 0.6f);
    	light_shader.SetVec3f("lightColor", 1.0f, 1.0f, 1.0f);
    	light_shader.SetUniform1f("ambientStrength", 0.1f);

    	Shader shader = Shader("res/shaders/BasicVertex.glsl", "res/shaders/BasicFragment.glsl");
    	shader.Bind();
    	shader.SetVec4f("u_Color", 0.6f, 0.6f, 0.6f, 1.0f);

    	Texture texture("res/textures/tex.jpg");
    	texture.Bind();
    	// shader.SetUniform1i("u_Texture", 0);

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


    	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 20.0f);
    	glm::vec3 lightPos = glm::vec3(3.0f, 2.0f, 25.0f);
    	auto model_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    	float scale = 1.0f;
    	while (!glfwWindowShouldClose(window))
    	{
    		renderer.Clear();

    		//imgui initialization
    		ImGui_ImplOpenGL3_NewFrame();
    		ImGui_ImplGlfw_NewFrame();
    		ImGui::NewFrame();

    		// shader.Bind();
    		// {
    		// 	model = glm::translate(glm::mat4(1.0f), translation);
    		// 	model = glm::rotate(model,static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0.0f));
    		// 	model_scale = glm::vec3(scale, scale, scale);
    		// 	model = glm::scale(model, model_scale);
    		// 	shader.SetUniformMat4f("model", model);
    		// 	shader.SetUniformMat4f("view", view);
    		// 	shader.SetUniformMat4f("projection", proj);
    		// 	renderer.Draw(va, ib, shader);
    		// }
    		{
    			light_shader.Bind();
				model = glm::translate(glm::mat4(1.0f), translation);
    			model = glm::rotate(model,static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0.0f));
    			model_scale = glm::vec3(scale, scale, scale);
    			model = glm::scale(model, model_scale);
    			light_shader.SetUniformMat4f("model", model);
    			light_shader.SetUniformMat4f("view", view);
    			light_shader.SetUniformMat4f("projection", proj);
    			light_shader.SetVec3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
    			renderer.Draw(va, ib, light_shader);
    		}
    		//imgui
    		{
    			ImGui::SliderFloat3("position", &translation.x, -100.0f, 100.0f);
    			ImGui::SliderFloat("Scale", &scale, 0.0f, 5.0f);
    			ImGui::SliderFloat3("lightPos", &lightPos.x, -40.0f, 40.0f);
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
