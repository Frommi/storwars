#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>

#include "input_handler.h"

class Application : public InputHandler {
public:
    Application();

    void initApp();
    void run();
    void terminateApp();

private:
    static void errorCallback(int error, const char* description) {
        fputs(description, stderr);
    }

    GLFWwindow* window_;
};
