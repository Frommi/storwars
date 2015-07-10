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

class Camera {
    glm::vec3 position;
    glm::quat direction; // this quat is rotating
};

class WorldPipeline {
public:
    WorldPipeline();

    void setMove(const glm::vec3& move_vector);
    void setRotation(const glm::quat& rotation);
    void setProjection(const GLFWwindow* window);
    void setCamera(const Camera& camera);

    ~WorldPipeline() = default;

private:
    void setCameraMove();
    void setCameraRotate();

private:
    glm::mat4 move_, rotate_, projection_, camera_rotate_, camera_move_;
};
