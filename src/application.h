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
};
