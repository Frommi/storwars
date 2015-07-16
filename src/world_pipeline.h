#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(const glm::vec3& pos):
        position_(pos), 
        direction_(glm::quat(0.0f, 0.0f, 0.0f, 1.0f)) 
    {}

    Camera(const glm::vec3& pos, const glm::quat& dir):
        position_(pos), 
        direction_(dir) 
    {}

    ~Camera() = default;

    void set_position(const glm::vec3& pos) { position_ = pos; }
    void updatePositionBy(const glm::vec3& pos) { position_ += pos; }

    void rotatePitchYawRoll(float pitch, float yaw, float roll);

    inline glm::vec3 get_position()  const { return position_;  }
    inline glm::quat get_direction() const { return direction_; }

    glm::vec3 get_target() const;
    glm::vec3 get_up() const;
    glm::vec3 get_right() const;

private:
    glm::vec3 position_;
    glm::quat direction_;
};

class WorldPipeline {
public:
    WorldPipeline();

    ~WorldPipeline() = default;

    void set_scale(const glm::vec3& scale);
    void set_rotation(const glm::quat& rotation);
    void set_move(const glm::vec3& move);
    void set_camera(const Camera& camera);
    void set_projection(const GLFWwindow* window);
    
    glm::mat4 get_WVP_matrix();

private:
    glm::mat4 scale_;
    glm::mat4 rotate_;
    glm::mat4 move_;
    glm::mat4 camera_move_;
    glm::mat4 camera_rotate_;
    glm::mat4 projection_;
};
