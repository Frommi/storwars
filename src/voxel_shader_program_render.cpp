#include "voxel_shader_program_render.h"

bool VoxelShaderProgramRender::initShaderProgram() {
    if (!this->preInit()) 
        return false;
    
    if (!this->addShader("voxel_shaders/vs.glsl", GL_VERTEX_SHADER)) 
        return false;

    if (!this->addShader("voxel_shaders/fs.glsl", GL_FRAGMENT_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    W_matrix_uniform_        = getUniformLocation("u_W");
    VP_matrix_uniform_       = getUniformLocation("u_VP");

    if (W_matrix_uniform_        == INVALID_LOCATION ||
        VP_matrix_uniform_       == INVALID_LOCATION) {

        fprintf(stderr, "invalid uniform location\n");
        return false;
    }    

    return true;
}
