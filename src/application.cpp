#include "application.h"

#include "world_pipeline.h"

Application::Application() {}

void Application::initApp() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwSetErrorCallback(errorCallback);

    window_ = glfwCreateWindow(1200, 675, "SToR Wars", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "glew init error");
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLint a, b;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &a);
    glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &b);
    fprintf(stderr, "%d x %d\n", a, b);

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
    //static_mesh->loadFromFile("models/sphere.obj");
    //static_mesh->loadFromFile("models/city/city.obj");
    static_mesh->loadFromFile("models/island/island.obj");

    glm::vec3 p = glm::vec3(0, 0, 0);
    Camera camera(glm::vec3(20.0, -20.0, -60.0));
    float dt =  1.0f / 3.0f;
    while (!glfwWindowShouldClose(window_)) {
        if (this->isKeyboardKeyPressed(GLFW_KEY_ESCAPE)) 
            glfwSetWindowShouldClose(window_, GL_TRUE);

        if (this->isKeyboardKeyPressed(GLFW_KEY_SPACE))
        	p = glm::vec3(0, 0, 0);

        glm::vec3 f = glm::vec3(0, 0, 0);

        if (this->isKeyboardKeyPressed(GLFW_KEY_A)) 
            f += -camera.get_right();

        if (this->isKeyboardKeyPressed(GLFW_KEY_D)) 
            f += camera.get_right();

        if (this->isKeyboardKeyPressed(GLFW_KEY_S)) 
            f += -camera.get_target();

        if (this->isKeyboardKeyPressed(GLFW_KEY_W)) 
            f += camera.get_target();

        if (glm::dot(f, f) > 1e-6) f = glm::normalize(f) / 100.0f;

        p += f;

        glm::vec3 v = p / static_cast<float>(sqrt(1 + glm::dot(p, p)));
        camera.updatePositionBy(v * dt);

        float roll = 0.0f;
        if (this->isKeyboardKeyPressed(GLFW_KEY_Q)) 
            roll += 0.1f;

        if (this->isKeyboardKeyPressed(GLFW_KEY_E)) 
            roll -= 0.1f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int c = 0;
        roll *= 1000.0f / ((c + 1) * (c + 1) * (c + 1));
        for (int i = -c; i <= c; ++i) {
            for (int j = -c; j <= c; ++j) {
                for (int k = -c; k <= c; ++k) {
                    WorldPipeline pipe;
                    glm::vec2 d_mouse = -pullCursorDelta();
                    camera.rotatePitchYawRoll(-d_mouse.y, -d_mouse.x, roll);
                    pipe.set_camera(camera);
                    pipe.set_projection(window_);
                    pipe.set_scale(glm::vec3(0.2f, 0.2f, 0.2f));
                    glm::vec3 eye_pos = glm::vec3(i * 50, j * 50, k * 50);
                    pipe.set_move(eye_pos);

                    static_shader_program->set_W_matrix_uniform(pipe.get_W_matrix());
                    static_shader_program->set_VP_matrix_uniform(pipe.get_VP_matrix());
                    static_shader_program->set_obs_pos_uniform(camera.get_position());
					static_shader_program->set_obs_impulse_uniform(p);
					static_shader_program->set_diffuse_texture_uniform(0);
                    static_mesh->render();
                }
            }
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}
