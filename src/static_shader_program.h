#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shader_program.h"

class StaticShaderProgram : public ShaderProgram {
public:
    StaticShaderProgram() {}

    bool initShaderProgram();

    inline void set_W_matrix_uniform(const glm::mat4& W) const {
        glUniformMatrix4fv(W_matrix_uniform_, 1, GL_TRUE, (const GLfloat*) &(W[0][0]));
    }

    inline void set_VP_matrix_uniform(const glm::mat4& VP) const {
        glUniformMatrix4fv(VP_matrix_uniform_, 1, GL_TRUE, (const GLfloat*) &(VP[0][0]));
    }

    inline void set_obs_pos_uniform(const glm::vec3& obs_pos) const {
        glUniform3fv(obs_pos_uniform_, 1, (const GLfloat*) &(obs_pos[0]));
    }

    inline void set_obs_impulse_uniform(const glm::vec3& obs_impulse) const {
        glUniform3fv(obs_impulse_uniform_, 1, (const GLfloat*) &(obs_impulse[0]));
    }

    inline void set_diffuse_texture_uniform(int index) {
    	glUniform1i(diffuse_texture_uniform_, index);
    }

private:
    GLuint W_matrix_uniform_;
    GLuint VP_matrix_uniform_;

    GLuint obs_pos_uniform_;
    GLuint obs_impulse_uniform_;
    
    GLuint diffuse_texture_uniform_;
};
