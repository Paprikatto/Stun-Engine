#pragma once
#include <glm/glm.hpp>

class Camera
{
private:
    float m_fov;
    glm::vec3 m_position;
    glm::mat4 m_view_matrix;
    int m_viewport_width;
    int m_viewport_height;
    glm::mat4 m_projection_matrix;
public:
    explicit Camera(glm::vec3 position, float fov, int width, int height);
    void set_fov(float fov);
    [[nodiscard]] float get_fov() const { return m_fov; }
    void set_position(glm::vec3 position);
    [[nodiscard]] glm::vec3 get_position() const { return m_position;}
    [[nodiscard]] glm::mat4 get_view_matrix() const { return m_view_matrix; }
    [[nodiscard]] glm::mat4 get_projection_matrix() const { return m_projection_matrix; }
};
