#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

#include "shader_program.h"

class VoxelShaderProgramGenerate : public ShaderProgram {
public:
    VoxelShaderProgramGenerate() {}

    bool initShaderProgram();

    inline void set_size_uniform(const glm::uvec3& size) const {
        glUniform3ui(size_uniform_, size.x, size.y, size.z);
    }

    inline void set_time_uniform(const float t) const {
        glUniform1f(time_uniform_, t);
    }

private:
    GLuint size_uniform_;
    GLuint time_uniform_;
};
