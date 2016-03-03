#include "application.h"

const int size = 128;

unsigned int seed = 239017;

noisepp::PerlinModule perlin;
noisepp::TurbulenceModule turbulence;

noisepp::Pipeline3D* noise_pipeline;
noisepp::ElementID id;
noisepp::PipelineElement3D* element;
noisepp::PipelineElement3D* turbulence_element;
noisepp::Cache* cache;

float getFrame(float x, float a, float b) {
    return std::exp(-std::min(x - a, b - x) * 100.0f) * 3.0f;
}

float planet1(const glm::vec3& pos) {
    glm::vec3 p = pos - glm::vec3(0.5f);
    p = p + glm::normalize(p) * std::sin(glm::length(p) * 22.0f) / 25.0f;

    float noise = element->getValue(p.x, p.y, p.z, cache);
    noise *= 1.0 + std::sin(glm::length(p) * 25.0f);
    float frame = getFrame(pos.x, 0.0f, 1.0f) + getFrame(pos.y, 0.0f, 1.0f) + getFrame(pos.z, 0.0f, 1.0f);
//    float frame = 0;
    float sphere = (glm::distance(pos, glm::vec3(0.5f)) - 0.4f) * 2.0f;
    //float sphere = 0;
    float result = sphere + noise + frame;
    //noise_pipeline->cleanCache(cache);
    return result;
}

float planet2(const glm::vec3& pos) {
    glm::vec3 p = pos - glm::vec3(0.5f);
    p = p - glm::normalize(p) * std::sin(glm::length(p) * 22.0f) / 25.0f;

    float noise = element->getValue(p.x, p.y, p.z, cache);
    noise *= 1.0 + std::sin(glm::length(p) * 25.0f);
    float frame = getFrame(pos.x, 0.0f, 1.0f) + getFrame(pos.y, 0.0f, 1.0f) + getFrame(pos.z, 0.0f, 1.0f);
//    float frame = 0;
    float sphere = (glm::distance(pos, glm::vec3(0.5f)) - 0.4f) * 4.0f;
    //float sphere = 0;
    float result = sphere + noise + frame;
    //noise_pipeline->cleanCache(cache);
    return result;
}

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
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &c);
    fprintf(stderr, "texture array max size = %d\n", c);

    GLint d;
    glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &d);
    fprintf(stderr, "shared memory in bytes = %d\n", d);

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

    if (this->isKeyboardKeyPressed(GLFW_KEY_R)) {
        seed = seed * 239 + 179;
        seed %= 1000003;

        noise_pipeline->freeCache(cache);
        cache = 0;
        delete noise_pipeline;
        noise_pipeline = 0;

        perlin.setFrequency(7.0f);
        perlin.setOctaveCount(4);
        perlin.setSeed(seed);
        perlin.setScale(1.0f);

        turbulence.setFrequency(1.3f);
        turbulence.setSeed(seed * 13 + 17);
        turbulence.setPower(0.3f);
        turbulence.setRoughness(3);
        turbulence.setSourceModule(0, perlin);

        noise_pipeline = new noisepp::Pipeline3D;
        id = turbulence.addToPipeline(noise_pipeline);
        element = noise_pipeline->getElement(id);
        turbulence_element = noise_pipeline->getElementPtr(turbulence);
        cache = noise_pipeline->createCache();

        VoxelGenerator* vg = new VoxelGenerator;
        static_mesh_ = vg->generateMesh(glm::ivec3(size), planet1);
        delete vg;
    }
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
                pipe.set_move(-camera_->get_position());

                voxel_shader_program_stupid_cs_->enable();
                voxel_shader_program_stupid_cs_->set_size_uniform(glm::uvec3(size, size, size));

                //fprintf(stderr, "%d -- no error; %d -- my errors\n", GL_NO_ERROR, glGetError());

                GLuint* zero = new GLuint;
                *zero = 0;
                glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), zero, GL_STATIC_DRAW);
                delete zero;


                glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_pos);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_pos);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_ind);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_ind);
                glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, tri_cnt);
                glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, tri_cnt);
                glDispatchCompute(size / 8, size / 8, size / 8);
                glFinish();

                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

                int counter = *((GLuint*) glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, 1 * sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT));
                glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
                fprintf(stderr, "%d\n", counter);

/*
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_ind);
                GLuint* inds = (GLuint*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 6 * counter * sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
                glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

                GLuint min_i = inds[0];
                GLuint max_i = inds[0];
                for (int i = 0; i < 6 * counter * sizeof(GLuint); ++i) {
                    //fprintf(stderr, "[%u]", inds[i]);
                    if (min_i > inds[i]) min_i = inds[i];
                    if (max_i < inds[i]) max_i = inds[i];
                }
                fprintf(stderr, "min_i: %u, max_i: %u\n", min_i, max_i);
*/
                
                voxel_shader_program_render_->enable();
                voxel_shader_program_render_->set_W_matrix_uniform(pipe.get_W_matrix());
                voxel_shader_program_render_->set_VP_matrix_uniform(pipe.get_VP_matrix());
                

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, SSBO_pos);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (const GLvoid*) 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SSBO_ind);
                glDrawElements(GL_TRIANGLES, counter * 6, GL_UNSIGNED_INT, 0);
                glDisableVertexAttribArray(0);

/*
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, SSBO_pos);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (const GLvoid*) 0);
                glDrawArrays(GL_POINTS, 0, size * size * size);
                glDisableVertexAttribArray(0);
*/

/*
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
*/
            }
        }
    }
    
    glfwSwapBuffers(window_);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::run() {  // Temporary
    /*
    static_shader_program_ = new StaticShaderProgram();
    static_shader_program_->initShaderProgram();
    static_shader_program_->enable();
    */

    voxel_shader_program_stupid_cs_ = new VoxelShaderProgramStupidCS();
    voxel_shader_program_stupid_cs_->initShaderProgram();
    voxel_shader_program_stupid_cs_->enable();

    glGenBuffers(1, &SSBO_pos);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_pos);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * size * size * size, NULL, GL_STREAM_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_pos);
    //glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, SSBO_pos, 0, size*size*size*3*4);

    glGenBuffers(1, &SSBO_ind);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_ind);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * size * size * size * 6 * 3, NULL, GL_STREAM_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_ind);
    //glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, SSBO_ind, 0, size*size*size*3*3*4);

    GLuint* zero = new GLuint;
    *zero = 0;
    glGenBuffers(1, &tri_cnt);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, tri_cnt);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), zero, GL_DYNAMIC_READ);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, tri_cnt);
    delete zero;


    voxel_shader_program_render_ = new VoxelShaderProgramRender();
    voxel_shader_program_render_->initShaderProgram();

    perlin.setFrequency(4.7f);
    perlin.setOctaveCount(4);
    perlin.setSeed(seed);
    perlin.setScale(1.0f);

    turbulence.setFrequency(1.1f);
    turbulence.setSeed(seed * 13 + 17);
    turbulence.setPower(0.3f);
    turbulence.setRoughness(3);
    turbulence.setSourceModule(0, perlin);

    noise_pipeline = new noisepp::Pipeline3D;
    id = turbulence.addToPipeline(noise_pipeline);
    element = noise_pipeline->getElement(id);
    turbulence_element = noise_pipeline->getElementPtr(turbulence);
    cache = noise_pipeline->createCache();

    //static_mesh_ = new StaticMesh();
    //static_mesh_->loadFromFile("models/sphere.obj");
    //static_mesh_->loadFromFile("models/city/", "city.obj");
    //static_mesh_->loadFromFile("models/island/", "island.obj");
    //static_mesh_->loadFromFile("models/sphere/sphere/", "sphere.obj");
    //static_mesh_->loadFromFile("models/big_sphere/", "sphere.obj");
    //static_mesh_->loadFromFile("models/monkey/", "monkey.obj");
    //static_mesh_->loadFromFile("models/torus/", "2torus.obj");
    //static_mesh_->loadFromFile("models/wheel/", "wheel1.obj");
    //static_mesh_->loadFromFile("models/big_dice/", "dice.obj");

/*

    VoxelGenerator* vg = new VoxelGenerator;
    static_mesh_ = vg->generateMesh(glm::ivec3(size), planet1);
    delete vg;

*/

    //p_ = glm::vec3(-0.95f / sqrt(1.0f - 0.95f * 0.95f), 0, 0);
    p_ = glm::vec3(0.0f);
    //camera_ = new Camera(glm::vec3(0.0, 3.0, -3.0));
    camera_ = new Camera(glm::vec3(0.0, 0.0, 0.0));

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
//          printf("ifr_time: %f, self_time: %f\n", ifr_time, t);
            cnt = 0;
            dt -= 1.0f;
        }
    }
}
