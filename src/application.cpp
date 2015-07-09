#include "Application.h"

Application::Application() {}

void Application::initApp() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwSetErrorCallback(errorCallback);

    window_ = glfwCreatewindow_(640, 480, "SToR Wars", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window_);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "glew init error");
        exit(EXIT_FAILURE);
    }
    
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    this->initInputHandler(window_);
}

void Application::terminateApp() {
    glfwTerminate();
}

void Application::run() {
    
}
