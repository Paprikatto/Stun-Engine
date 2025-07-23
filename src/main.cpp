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

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <filesystem>

#include "Camera.h"

//shaders
#include "lit_vert.h"
#include "lit_frag.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	constexpr int width = 640;
	constexpr int height = 480;
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

    	GL_CALL(glEnable(GL_BLEND))
    	glEnable(GL_DEPTH_TEST);
    	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    	
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

    	Camera camera(glm::vec3(0.0f, 0.0f, 23.0f), 45.0f, width, height);
    	auto model = glm::mat4(1.0f);

    	Shader lit_shader = Shader(___res_shaders_lit_vert, ___res_shaders_lit_vert_len,
    		___res_shaders_lit_frag, ___res_shaders_lit_frag_len);
    	lit_shader.Bind();
    	lit_shader.SetVec3f("objectColor", 0.6f, 0.6f, 0.6f);
    	lit_shader.SetVec3f("lightColor", 1.0f, 1.0f, 1.0f);
    	lit_shader.SetUniform1f("ambientStrength", 0.1f);
    	
    	Texture texture("res/textures/tex.jpg");
    	texture.Bind();
    	// shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
    	lit_shader.Unbind();
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

    	double lastFrame = 0.0;
    	while (!glfwWindowShouldClose(window))
    	{
    		renderer.Clear();

    		//imgui initialization
    		ImGui_ImplOpenGL3_NewFrame();
    		ImGui_ImplGlfw_NewFrame();
    		ImGui::NewFrame();

    		//delta time
    		double currentFrame = glfwGetTime();
    		double deltaTime = currentFrame - lastFrame;
    		lastFrame = currentFrame;

    		//camera movement
    		glm::vec3 camera_movement(0.0f);
    		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				camera_movement.z += -1.0f;
    		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    			camera_movement.z += 1.0f;
    		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				camera_movement.x += -1.0f;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				camera_movement.x += 1.0f;

			float speed = static_cast<float>(10.0 * deltaTime);
			camera_movement.x *= speed;
			camera_movement.y *= speed;
			camera_movement.z *= speed;
			camera.set_position(camera.get_position() + camera_movement);
    			
    			
    		
			//rendering
    		{
    			lit_shader.Bind();
				model = glm::translate(glm::mat4(1.0f), translation);
    			model = glm::rotate(model,static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0.0f));
    			model_scale = glm::vec3(scale, scale, scale);
    			model = glm::scale(model, model_scale);
    			lit_shader.SetUniformMat4f("model", model);
    			lit_shader.SetUniformMat4f("view", camera.get_view_matrix());
    			lit_shader.SetUniformMat4f("projection", camera.get_projection_matrix());
    			glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
    			lit_shader.SetUniformMat3f("normalMatrix", normal_matrix);
    			lit_shader.SetVec3f("lightPos", lightPos);
    			lit_shader.SetVec3f("viewPos", camera.get_position());
    			renderer.Draw(va, ib, lit_shader);
    		}
    		//imgui
    		{
    			ImGui::SliderFloat3("position", &translation.x, -30.0f, 30.0f);
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
