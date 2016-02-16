#include "voxel_shader_program.h"

bool VoxelShaderProgram::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("voxel_shaders/vs.glsl", GL_VERTEX_SHADER)) 
        return false;

    if (!this->addShader("voxel_shaders/gs.glsl", GL_GEOMETRY_SHADER)) 
        return false;

    if (!this->addShader("voxel_shaders/fs.glsl", GL_FRAGMENT_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    W_matrix_uniform_        = getUniformLocation("u_W");
    VP_matrix_uniform_       = getUniformLocation("u_VP");

    obs_pos_uniform_         = getUniformLocation("u_ObsPos");
    obs_impulse_uniform_     = getUniformLocation("u_ObsImpulse");
    inverse_g_uniform_       = getUniformLocation("u_InverseG");


    if (W_matrix_uniform_        == INVALID_LOCATION ||
        VP_matrix_uniform_       == INVALID_LOCATION ||
        obs_pos_uniform_         == INVALID_LOCATION ||
        obs_impulse_uniform_     == INVALID_LOCATION ||
        diffuse_texture_uniform_ == INVALID_LOCATION ||
        inverse_g_uniform_       == INVALID_LOCATION) {

        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
