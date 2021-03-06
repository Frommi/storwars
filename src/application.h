#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <cstring>

#include "input_handler.h"
#include "static_shader_program.h"
#include "static_mesh.h"
#include "world_pipeline.h"

class Application : public InputHandler {
public:
    Application();

    void initApp();
    void run();
    void terminateApp();

private:
    static void errorCallback(int, const char* description) {
        fputs(description, stderr);
    }

    GLFWwindow* window_;

    void updateInput(float dt);
    void tick(float dt, float& ifr_time);
    void render(float ifr_time);

    Camera* camera_;
    glm::vec3 p_;
    glm::vec3 f_;
    StaticMesh* static_mesh_;
    StaticShaderProgram* static_shader_program_;
    float roll_;

    int frame_num_;
    float dt_;
    float prev_time_;
    float ifr_time_;

    const int FPS_TICK = 1000;
};
