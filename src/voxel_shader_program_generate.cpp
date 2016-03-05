#include "voxel_shader_program_generate.h"

bool VoxelShaderProgramGenerate::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("voxel_shaders/cs.glsl", GL_COMPUTE_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    size_uniform_ = getUniformLocation("size");
    time_uniform_ = getUniformLocation("time");

    if (size_uniform_ == INVALID_LOCATION ||
        time_uniform_ == INVALID_LOCATION) {
        
        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
