#include "static_shader_program.h"

bool StaticShaderProgram::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("shaders/vs.glsl", GL_VERTEX_SHADER)) 
        return false;

    if (!this->addShader("shaders/tcs.glsl", GL_TESS_CONTROL_SHADER)) 
        return false;

    if (!this->addShader("shaders/tes.glsl", GL_TESS_EVALUATION_SHADER)) 
        return false;

    if (!this->addShader("shaders/gs.glsl", GL_GEOMETRY_SHADER)) 
        return false;

    if (!this->addShader("shaders/fs.glsl", GL_FRAGMENT_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    W_matrix_uniform_    	 = getUniformLocation("u_W");
    VP_matrix_uniform_   	 = getUniformLocation("u_VP");

    obs_pos_uniform_     	 = getUniformLocation("u_ObsPos");
    obs_impulse_uniform_ 	 = getUniformLocation("u_ObsImpulse");
    inverse_g_uniform_       = getUniformLocation("u_InverseG");
    obs_ifr_time_uniform_    = getUniformLocation("u_ObsIFRTime");

    map_size_uniform_        = getUniformLocation("u_MapSize");

    diffuse_texture_uniform_ = getUniformLocation("diffuseTexture");

    obj_velocity_uniform_    = getUniformLocation("u_ObjVelocity");

    if (W_matrix_uniform_        == INVALID_LOCATION ||
    	VP_matrix_uniform_       == INVALID_LOCATION ||
    	obs_pos_uniform_         == INVALID_LOCATION ||
    	obs_impulse_uniform_     == INVALID_LOCATION ||
    	diffuse_texture_uniform_ == INVALID_LOCATION ||
    	inverse_g_uniform_       == INVALID_LOCATION ||
    	obs_ifr_time_uniform_    == INVALID_LOCATION ||
    	map_size_uniform_        == INVALID_LOCATION ||
    	obj_velocity_uniform_    == INVALID_LOCATION) {

        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
