#pragma once
#include <glm.hpp>

class Camera
{
private:
    float m_fov;
    float m_mouse_sensitivity = 0.1f;
    glm::vec3 m_position;

    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_yaw = -180.0f; // Facing towards negative Z by default
    float m_pitch = 0.0f;

    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;

    int m_viewport_width;
    int m_viewport_height;

public:
    explicit Camera(glm::vec3 position, float fov, int width, int height);
    void set_fov(float fov);
    [[nodiscard]] float get_fov() const { return m_fov; }
    void set_position(glm::vec3 position);
    [[nodiscard]] glm::vec3 get_position() const { return m_position;}
    [[nodiscard]] glm::mat4 get_view_matrix() const { return m_view_matrix; }
    [[nodiscard]] glm::mat4 get_projection_matrix() const { return m_projection_matrix; }
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch);
    void update_camera_view();
};
