#include "world_pipeline.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>

void Camera::rotatePitchYawRoll(float pitch, float yaw, float roll) {
    pitch /= 300.0f;  // Temporal magic constant -- mouse sensitivity
    yaw /= 300.0f;    // Temporal magic constant -- mouse sensitivity
    roll /= 1000.0f;   // Temporal magic constant -- mouse sensitivity

    glm::quat rotation = glm::quat(glm::vec3(pitch, yaw, roll));
    direction_ = direction_ * rotation;
}

glm::vec3 Camera::get_target() const {
    glm::vec4 target = glm::mat4_cast(direction_) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    return glm::vec3(target.x, target.y, target.z);
}

glm::vec3 Camera::get_up() const {
    glm::vec4 up = glm::mat4_cast(direction_) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    return glm::vec3(up.x, up.y, up.z);
}

glm::vec3 Camera::get_right() const {
    glm::vec4 right = glm::mat4_cast(direction_) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(right.x, right.y, right.z);
}

WorldPipeline::WorldPipeline() {
    scale_           = glm::mat4(1.0f);
    rotate_          = glm::mat4(1.0f);
    move_            = glm::mat4(1.0f);
    camera_move_     = glm::mat4(1.0f);
    camera_rotate_   = glm::mat4(1.0f);
    projection_      = glm::mat4(1.0f);
}

void WorldPipeline::set_scale(const glm::vec3& scale) {
    scale_ = glm::mat4(1.0f);
    scale_[0][0] = scale[0];
    scale_[1][1] = scale[1];
    scale_[2][2] = scale[2];
}

void WorldPipeline::set_rotation(const glm::quat& rotation) {
    rotate_ = glm::mat4_cast(rotation);
}

void WorldPipeline::set_move(const glm::vec3& move_vector) {
    move_ = glm::transpose(glm::translate(move_vector));
}

void WorldPipeline::set_camera(const Camera& camera) {
    camera_move_ = glm::transpose(glm::translate(-camera.get_position()));
    camera_rotate_ = glm::mat4_cast(camera.get_direction());
}

void WorldPipeline::set_projection(const GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);

    float FOV = 71.0f;       // magic constant -- field of view
    float zNear = 1.0f;      // magic constant -- near plane
    float zFar = 100000.0f;  // magic constant -- far plane

    projection_ = glm::mat4(1.0f);
    float ar = static_cast<float>(width) / static_cast<float>(height);
    float tanFOV = tan((FOV / 2.0) * M_PI / 180.0f);

    projection_ = glm::mat4(0.0f);
    projection_[0][0] = 1.0f / (ar * tanFOV);
    projection_[1][1] = 1.0f / tanFOV;
    projection_[2][2] = (-zNear - zFar) / (zNear - zFar);
    projection_[2][3] = (2.0f * zFar * zNear) / (zNear - zFar);
    projection_[3][2] = 1.0f;
}

glm::mat4 WorldPipeline::get_WVP_matrix() {
    return scale_ * rotate_ * move_ * camera_move_ * camera_rotate_ * projection_;
}

glm::mat4 WorldPipeline::get_W_matrix() {
    return scale_ * rotate_ * move_;
}

glm::mat4 WorldPipeline::get_VP_matrix() {
    return camera_rotate_ * projection_;
}
