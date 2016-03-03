#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

#include "shader_program.h"

class VoxelShaderProgramStupidCS : public ShaderProgram {
public:
    VoxelShaderProgramStupidCS() {}

    bool initShaderProgram();

    inline void set_size_uniform(const glm::uvec3& size) const {
        glUniform3ui(size_uniform_, size.x, size.y, size.z);
    }

private:
    GLuint size_uniform_;

};
