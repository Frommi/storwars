#include "application.h"

#include "world_pipeline.h"

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

void Application::run() {  // Temporary
    StaticShaderProgram* static_shader_program = new StaticShaderProgram();
    static_shader_program->initShaderProgram();
    static_shader_program->enable();

    StaticMesh* static_mesh = new StaticMesh();
    static_mesh->loadFromFile("models/sphere.obj");

    Camera camera(glm::vec3(0.0, 0.0, -20.0));
    while (!glfwWindowShouldClose(window_)) {
        if (this->isKeyboardKeyPressed(GLFW_KEY_ESCAPE)) 
            glfwSetWindowShouldClose(window_, GL_TRUE);

        if (this->isKeyboardKeyPressed(GLFW_KEY_A)) 
            camera.updatePositionBy(-camera.get_right());

        if (this->isKeyboardKeyPressed(GLFW_KEY_D)) 
            camera.updatePositionBy(camera.get_right());

        if (this->isKeyboardKeyPressed(GLFW_KEY_S)) 
            camera.updatePositionBy(-camera.get_target());

        if (this->isKeyboardKeyPressed(GLFW_KEY_W)) 
            camera.updatePositionBy(camera.get_target());

        float roll = 0.0f;
        if (this->isKeyboardKeyPressed(GLFW_KEY_Q)) 
            roll -= 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_E)) 
            roll += 0.1f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = -3; i <= 3; ++i) {
            for (int j = -3; j <= 3; ++j) {
                for (int k = -3; k <= 3; ++k) {
                    WorldPipeline pipe;
                    glm::vec2 d_mouse = -pullCursorDelta();
                    camera.rotatePitchYawRoll(-d_mouse.y, -d_mouse.x, roll);
                    pipe.set_camera(camera);
                    pipe.set_projection(window_);
                    pipe.set_move(glm::vec3(i * 50, j * 50, k * 50));

                    static_shader_program->set_WVP_matrix_uniform(pipe.get_WVP_matrix());
                    static_mesh->render();
                }
            }
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}
