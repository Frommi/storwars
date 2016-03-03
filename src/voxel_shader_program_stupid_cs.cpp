#include "voxel_shader_program_stupid_cs.h"

bool VoxelShaderProgramStupidCS::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("voxel_shaders/stupid_cs.glsl", GL_COMPUTE_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    size_uniform_ = getUniformLocation("size");

    if (size_uniform_ == INVALID_LOCATION) {
        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
