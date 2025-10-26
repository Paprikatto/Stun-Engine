#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "Renderer.h"
#include "Shader.h"
#include "Model.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"

//shaders
#include "InputServer.h"
#include "lit_vert.h"
#include "lit_frag.h"

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

	constexpr int width = 1600;
	constexpr int height = 900;
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
    	GL_CALL(glEnable(GL_BLEND))
    	GL_CALL(glEnable(GL_DEPTH_TEST));
    	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), 45.0f, width, height);


    	//shader setup
    	Shader lit_shader = Shader(___res_shaders_lit_vert, ___res_shaders_lit_vert_len,
    		___res_shaders_lit_frag, ___res_shaders_lit_frag_len);
    	lit_shader.Bind();
    	lit_shader.SetVec3f("lightColor", 1.0f, 1.0f, 1.0f);
    	lit_shader.SetUniform1f("ambientStrength", 0.1f);
    	lit_shader.Unbind();

    	Model model("models/car/scene.gltf", lit_shader);

    	Renderer renderer;
    	renderer.SetBackgroundColor(glm::vec3(0.3f, 0.3f, 0.3f));

    	InputServer::init(window);

    	//imgui setup
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();
    	ImGuiIO& io = ImGui::GetIO();
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    	ImGui_ImplGlfw_InitForOpenGL(window, true);
    	ImGui_ImplOpenGL3_Init();
    	ImGui::StyleColorsDark();


    	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 2.0f);
    	glm::vec3 lightPos = glm::vec3(3.0f, 2.0f, 25.0f);
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
    		// double fps = 1.0 / deltaTime;
    		// std::cout << "FPS: " << fps << std::endl;

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
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				camera_movement.y += 1.0f;
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				camera_movement.y += -1.0f;

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
			    auto mouse_vec = InputServer::getInstance()->get_mouse_delta();
				// std::cout<<mouse_vec.x<<" "<<mouse_vec.y<<std::endl;
				camera.process_mouse_movement(mouse_vec.x, mouse_vec.y, true);
			}


			float speed = static_cast<float>(5.0 * deltaTime);
			camera_movement.x *= speed;
			camera_movement.y *= speed;
			camera_movement.z *= speed;
			camera.set_position(camera.get_position() + camera_movement);
			camera.update_camera_view();

			//rendering
    		{
    			model.SetPosition(translation);
    			model.SetScale(scale);
    			model.RotateRadians(static_cast<float>(currentFrame), glm::vec3(0.0f, 0.2f, 0.0f));
    			lit_shader.Bind();
    			lit_shader.SetUniformMat4f("view", camera.get_view_matrix());
    			lit_shader.SetUniformMat4f("projection", camera.get_projection_matrix());
    			lit_shader.SetVec3f("lightPos", lightPos);
    			lit_shader.SetVec3f("viewPos", camera.get_position());
    			model.Draw();
    		}
    		//imgui
    		{
    			ImGui::SliderFloat3("position", &translation.x, -20.0f, 20.0f);
    			ImGui::SliderFloat("Scale", &scale, 0.0f, 50.0f);
    			ImGui::SliderFloat3("lightPos", &lightPos.x, -40.0f, 40.0f);
    		}
    		// imgui end
    		ImGui::Render();
    		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    		/* Swap front and back buffers */
    		glfwSwapBuffers(window);

            InputServer::getInstance()->FrameEnd();
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
