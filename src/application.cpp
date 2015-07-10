#include "application.h"

Application::Application() {}

void Application::initApp() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwSetErrorCallback(errorCallback);

    window_ = glfwCreateWindow(640, 480, "SToR Wars", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    this->initInputHandler(window_);
}

void Application::terminateApp() {
    glfwTerminate();
}

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))  // Temporary
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))  // Temporary

void Application::run() {  // Temporary
    StaticShaderProgram* static_shader_program = new StaticShaderProgram();
    static_shader_program->initShaderProgram();
    static_shader_program->enable();

    StaticMesh* static_mesh = new StaticMesh();
    static_mesh->loadFromFile("sphere.obj");

    float xr = 0.0f, yr = 0.0f, scale = 0.05f;
    while (!glfwWindowShouldClose(window_)) {
        if (this->isKeyboardKeyPressed(GLFW_KEY_ESCAPE)) 
            glfwSetWindowShouldClose(window_, GL_TRUE);

        if (this->isKeyboardKeyPressed(GLFW_KEY_A)) 
            xr += 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_D)) 
            xr -= 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_S)) 
            yr += 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_W)) 
            yr -= 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_Q)) 
            scale *= 1.05f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_E)) 
            scale /= 1.05f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glm::mat4 m = glm::mat4(1.0f);
        float width = 640.0f;
        float height = 480.0f;
        float FOV = 71.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;
        float ar = ((float) width) / height;
        float tanFOV = tan(ToRadian(FOV / 2.0f));
        m[0][0] = 1.0f / (tanFOV * ar);
        m[1][1] = 1.0f / tanFOV;
        m[2][2] = (zNear + zFar) / (zFar - zNear);
        m[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
        m[3][2] = 1.0f;
        m[3][3] = 0.0f;

        glm::mat4 move = glm::mat4(1.0f);

        move[0][3] = xr;
        move[1][3] = yr;
        move[2][3] = -1.0f;

        glm::mat4 sca = glm::mat4(1.0f);

        sca[0][0] = scale;
        sca[1][1] = scale;
        sca[2][2] = scale;
        sca[3][3] = 1.0;

        static_shader_program->set_WVP_matrix_uniform(sca * m * move);

        static_mesh->render();

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}
