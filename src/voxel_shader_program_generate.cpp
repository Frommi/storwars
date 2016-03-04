#include "voxel_shader_program_generate.h"

bool VoxelShaderProgramGenerate::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("voxel_shaders/cs.glsl", GL_COMPUTE_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    size_uniform_ = getUniformLocation("size");
    offset_pos_uniform_ = getUniformLocation("offset_pos");

    if (size_uniform_ == INVALID_LOCATION ||
        offset_pos_uniform_ == INVALID_LOCATION) {

        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
