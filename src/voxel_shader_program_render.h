#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

#include "shader_program.h"

class VoxelShaderProgramRender : public ShaderProgram {
public:
    VoxelShaderProgramRender() {}

    bool initShaderProgram();

    inline void set_W_matrix_uniform(const glm::mat4& W) const {
        glUniformMatrix4fv(W_matrix_uniform_, 1, GL_TRUE, (const GLfloat*) &(W[0][0]));
    }

    inline void set_VP_matrix_uniform(const glm::mat4& VP) const {
        glUniformMatrix4fv(VP_matrix_uniform_, 1, GL_TRUE, (const GLfloat*) &(VP[0][0]));
    }

private:
    GLuint W_matrix_uniform_;
    GLuint VP_matrix_uniform_;
};
