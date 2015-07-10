#include "world_pipeline.h"
#include <iostream>
#include <stdio.h>

WorldPipeline::WorldPipeline() {
    move_            = glm::mat4(1.0f);
    rotate_          = glm::mat4(1.0f);
    projection_      = glm::mat4(1.0f);
    camera_rotate_   = glm::mat4(1.0f);
    camera_move_     = glm::mat4(1.0f);
}

void WorldPipeline::set_move(const glm::vec3& move_vector) {
    move_ = glm::translate(move_vector);
}

void WorldPipeline::set_rotation(const glm::quat& rotation) {
    rotate_ = glm::mat4_cast(rotation);
}

void WorldPipeline::set_projection(const GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);
    projection_ = glm::perspective(71.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    projection_ = glm::transpose(projection_);
}

void WorldPipeline::set_camera(const Camera& camera) {
//   camera_rotate_ = glm::mat4_cast(camera.direction);
    camera_move_ = glm::transpose(glm::translate(glm::mat4(1.0f), camera.position));
}

glm::mat4 WorldPipeline::get_WVP_matrix() {
    return projection_ * /*camera_rotate_ **/ camera_move_ * move_ * rotate_;
}
