#pragma once
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "glm.hpp"

class InputServer {
private:
	bool m_first_mouse_move = true;
	double m_mouse_x_pos = 0.0f;
	double m_mouse_y_pos = 0.0f;
	double m_mouse_x_delta = 0.0f;
	double m_mouse_y_delta = 0.0f;
	static InputServer* instance;

	static void cursor_position_callback([[maybe_unused]]GLFWwindow* window, double xpos, double ypos);
	explicit InputServer(GLFWwindow* window);
public:
	static void init(GLFWwindow* window) {
		if (instance == nullptr) {
			instance = new InputServer(window);
		} else {
			throw std::runtime_error("InputServer already initialized.");
		}
	}
    static InputServer* getInstance() {
        if (instance == nullptr) {
            throw std::runtime_error("InputServer not initialized.");
        }
        return instance;
    }
	[[nodiscard]] glm::vec2 get_mouse_delta() const {return {m_mouse_x_delta, m_mouse_y_delta};}
	void FrameEnd();

};
