#include "InputServer.h"

#include <iostream>

InputServer* InputServer::instance = nullptr;

InputServer::InputServer(GLFWwindow* window)
{
    if (instance != nullptr) {
        std::cout<<"InputServer already initialized!"<<std::endl;
        return;
    }
    instance = this;
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

void InputServer::cursor_position_callback([[maybe_unused]]GLFWwindow* window, double xpos, double ypos)
{
    if (instance->m_first_mouse_move)
    {
        instance->m_mouse_x_pos = xpos;
        instance->m_mouse_y_pos = ypos;
        instance->m_first_mouse_move = false;
        return;
    }
    instance->m_mouse_x_delta += xpos - instance->m_mouse_x_pos;
    instance->m_mouse_y_delta += instance->m_mouse_y_pos - ypos; // reversed since y-coordinates go from bottom to top
    instance->m_mouse_x_pos = xpos;
    instance->m_mouse_y_pos = ypos;

}

void InputServer::FrameEnd(){
    m_mouse_x_delta = 0;
    m_mouse_y_delta = 0;
}
