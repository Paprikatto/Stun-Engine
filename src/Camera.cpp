#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

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
    m_view_matrix = translate(glm::mat4(1.0f), m_position);
}
