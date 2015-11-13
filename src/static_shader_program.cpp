#include "static_shader_program.h"

bool StaticShaderProgram::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("shaders/vs.glsl", GL_VERTEX_SHADER)) 
        return false;

    if (!this->addShader("shaders/fs.glsl", GL_FRAGMENT_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    W_matrix_uniform_    	 = getUniformLocation("u_W");
    VP_matrix_uniform_   	 = getUniformLocation("u_VP");

    obs_pos_uniform_     	 = getUniformLocation("u_ObsPos");
    obs_impulse_uniform_ 	 = getUniformLocation("u_ObsImpulse");

    diffuse_texture_uniform_ = getUniformLocation("diffuseTexture");

    if (W_matrix_uniform_        == INVALID_LOCATION ||
    	VP_matrix_uniform_       == INVALID_LOCATION ||
    	obs_pos_uniform_         == INVALID_LOCATION ||
    	obs_impulse_uniform_     == INVALID_LOCATION ||
    	diffuse_texture_uniform_ == INVALID_LOCATION) {

        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
