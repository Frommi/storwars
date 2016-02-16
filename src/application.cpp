#include "application.h"

Application::Application() {}

void Application::initApp() {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwSetErrorCallback(errorCallback);
/*                                         
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	window_ = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
*/	                                       
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

    GLint c;
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &c);
    fprintf(stderr, "uniform size (in vec1) = %d\n", c);

    this->initInputHandler(window_);
}

void Application::terminateApp() {
    glfwTerminate();
}
                     
void Application::updateInput() {
    if (this->isKeyboardKeyPressed(GLFW_KEY_ESCAPE)) 
        glfwSetWindowShouldClose(window_, GL_TRUE);

    f_ = glm::vec3(0, 0, 0);

    if (this->isKeyboardKeyPressed(GLFW_KEY_A)) 
        f_ += -camera_->get_right();

    if (this->isKeyboardKeyPressed(GLFW_KEY_D)) 
        f_ += camera_->get_right();

    if (this->isKeyboardKeyPressed(GLFW_KEY_S)) 
        f_ += -camera_->get_target();

    if (this->isKeyboardKeyPressed(GLFW_KEY_W)) 
        f_ += camera_->get_target();

    if (glm::dot(f_, f_) > 1e-6) f_ = glm::normalize(f_) / 100.0f;
    if (this->isKeyboardKeyPressed(GLFW_KEY_Z)) f_ *= 3;

    if (this->isKeyboardKeyPressed(GLFW_KEY_SPACE))
      	f_ = -p_ * 0.1f;

    roll_ = 0.0f;
    if (this->isKeyboardKeyPressed(GLFW_KEY_Q)) 
        roll_ += 0.1f;

    if (this->isKeyboardKeyPressed(GLFW_KEY_E)) 
        roll_ -= 0.1f;
}

void Application::tick(float dt, float& ifr_time) {
	p_ += f_;        
    float g = 1.0f / static_cast<float>(sqrt(1.0f + glm::dot(p_, p_)));
    ifr_time += dt / g;
    camera_->updatePositionBy(p_ * dt);
}

void Application::render(float ifr_time) {
    roll_ *= 1000.0f;

    glm::vec2 d_mouse = -pullCursorDelta();
    camera_->rotatePitchYawRoll(-d_mouse.y, -d_mouse.x, roll_);	

    for (int i = 0; i <= 0; ++i) {
        for (int j = 0; j <= 0; ++j) {
            for (int k = 0; k <= 0; ++k) {
                WorldPipeline pipe;

                pipe.set_camera(*camera_);
                pipe.set_projection(window_);
                
                glm::vec3 eye_pos = glm::vec3(i, j, k) * 4.0f;
                pipe.set_move(eye_pos);

                static_shader_program_->set_obj_velocity_uniform(glm::vec3(0.0f, 0.0f, 0.0f));

                static_shader_program_->set_W_matrix_uniform(pipe.get_W_matrix());
                static_shader_program_->set_VP_matrix_uniform(pipe.get_VP_matrix());
                
	            static_shader_program_->set_obs_impulse_uniform(p_);
	            static_shader_program_->set_obs_pos_uniform(camera_->get_position());                	
                
				static_shader_program_->set_inverse_g_uniform(1.0f / (1.0f + sqrt(1.0f + glm::dot(p_, p_))));
				static_shader_program_->set_obs_ifr_time_uniform(ifr_time);
				static_shader_program_->set_map_size_uniform(1.0);
				static_shader_program_->set_diffuse_texture_uniform(0);
				
                static_mesh_->render();
            }
        }
    }

    
    glfwSwapBuffers(window_);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::run() {  // Temporary
    static_shader_program_ = new StaticShaderProgram();
    static_shader_program_->initShaderProgram();
    static_shader_program_->enable();

    static_mesh_ = new StaticMesh();
    //static_mesh_->loadFromFile("models/sphere.obj");
    static_mesh_->loadFromFile("models/city/", "city.obj");
    //static_mesh_->loadFromFile("models/island/", "island.obj");
    //static_mesh_->loadFromFile("models/sphere/sphere/", "sphere.obj");
    //static_mesh_->loadFromFile("models/big_sphere/", "sphere.obj");
    //static_mesh_->loadFromFile("models/monkey/", "monkey.obj");
    //static_mesh_->loadFromFile("models/torus/", "2torus.obj");
    //static_mesh_->loadFromFile("models/wheel/", "wheel1.obj");
    //static_mesh_->loadFromFile("models/big_dice/", "dice.obj");

    p_ = glm::vec3(-0.95f / sqrt(1.0f - 0.95f * 0.95f), 0, 0);
    camera_ = new Camera(glm::vec3(0.0, 3.0, -3.0));

    glfwSetTime(0.0f);
    int cnt = 0;
    float dt;
    float pref = static_cast<float>(glfwGetTime());
    float ifr_time = 0.0f;
    while (!glfwWindowShouldClose(window_)) {
    	updateInput();
    	float t = static_cast<float>(glfwGetTime());
    	dt += t - pref;
    	tick(t - pref, ifr_time);
    	pref = t;
    	render(ifr_time);
    	++cnt;      
    	if (dt > 1.0f) {
    		printf("frames pre sec: %d, milisec for frame: %d\n", cnt, 1000 / cnt);
//    		printf("ifr_time: %f, self_time: %f\n", ifr_time, t);
    		cnt = 0;
    		dt -= 1.0f;
    	}
    }
}
