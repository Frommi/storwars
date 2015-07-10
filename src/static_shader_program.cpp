#include "static_shader_program.h"

bool StaticShaderProgram::initShaderProgram() {
    if (!this->preInit()) 
        return false;

    if (!this->addShader("vs.glsl", GL_VERTEX_SHADER)) 
        return false;

    if (!this->addShader("fs.glsl", GL_FRAGMENT_SHADER)) 
        return false;

    if (!this->compileShaderProgram())
        return false;

    WVP_matrix_uniform_ = getUniformLocation("u_WVP");

    if (WVP_matrix_uniform_ == INVALID_LOCATION) {
        fprintf(stderr, "invalid uniform location\n");
        return false;
    }

    return true;
}
