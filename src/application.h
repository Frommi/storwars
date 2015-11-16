#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
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

    void updateInput();
    void tick(float dt);
    void render();

    Camera* camera_;
    glm::vec3 p_;
    glm::vec3 f_;
    StaticMesh* static_mesh_;
    StaticShaderProgram* static_shader_program_;
    float roll_;
};
