#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

struct Camera {
    Camera(const glm::vec3& pos, const glm::quat& dir):
        position(pos), direction(dir) {}
    ~Camera() = default;

    glm::vec3 position;
    glm::quat direction;
};

class WorldPipeline {
public:
    WorldPipeline();

    void set_move(const glm::vec3& move_vector);
    void set_rotation(const glm::quat& rotation);
    void set_projection(const GLFWwindow* window);
    void set_camera(const Camera& camera);

    glm::mat4 get_WVP_matrix();

    ~WorldPipeline() = default;

private:
    glm::mat4 move_, rotate_, projection_, camera_rotate_, camera_move_;
};
