#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shader_program.h"

class StaticShaderProgram : public ShaderProgram {
public:
    StaticShaderProgram() {}

    bool initShaderProgram();

    inline void set_WVP_matrix_uniform(const glm::mat4& WVP) const {
        glUniformMatrix4fv(WVP_matrix_uniform_, 1, GL_TRUE, (const GLfloat*) &(WVP[0][0]));
    }

private:
    GLuint WVP_matrix_uniform_;
};
