#include "world_pipeline.h"

WorldPipeline::WorldPipeline() {
    move_       = glm::mat4(1.0f);
    rotate_     = glm::mat4(1.0f);
    projection_ = glm::mat4(1.0f);
    camera_rotate_   = glm::mat4(1.0f);
    camera_move_     = glm::mat4(1.0f);
}

void WorldPipeline::setMove(const glm::vec3& move_vector) {
    move_ = glm::translate(move_vector);
}

void WorldPipeline::setRotation(const glm::quat& rotation) {
    rotate_ = glm::mat4_cast(rotation);
}

void WorldPipeline::setProjection(const GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);
    projection_ = glm::perspective(71.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}
