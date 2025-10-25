#include "Camera.h"
#include "gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, float fov, int width, int height): m_viewport_width(width), m_viewport_height(height)
{
    set_position(position);
    set_fov(fov);
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    m_projection_matrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_viewport_width) / static_cast<float>(m_viewport_height), 0.1f, 200.0f);
}

void Camera::set_position(glm::vec3 position)
{
    m_position = position;
}
void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;

    m_yaw -= x_offset;
    m_pitch += y_offset;

    if (constrain_pitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
}

void Camera::update_camera_view()
{
    glm::vec3 front;
    front.x = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
}
