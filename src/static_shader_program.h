#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
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

    inline void set_inverse_g_uniform(float inverse_g) const {
    	glUniform1f(inverse_g_uniform_, inverse_g);
    }

    inline void set_obs_ifr_time_uniform(float obs_ifr_time) const {
    	glUniform1f(obs_ifr_time_uniform_, obs_ifr_time);
    }

    inline void set_map_size_uniform(float map_size) const {
    	glUniform1f(map_size_uniform_, map_size);
    }

    inline void set_diffuse_texture_uniform(int index) const {
    	glUniform1i(diffuse_texture_uniform_, index);
    }

    inline void set_obj_velocity_uniform(const glm::vec3& obj_velocity) {
    	glUniform3fv(obj_velocity_uniform_, 1, (const GLfloat*) &(obj_velocity[0]));
    }

private:
    GLuint W_matrix_uniform_;
    GLuint VP_matrix_uniform_;

    GLuint obs_pos_uniform_;
    GLuint obs_impulse_uniform_;
    GLuint inverse_g_uniform_;
    GLuint obs_ifr_time_uniform_;

    GLuint map_size_uniform_;
    
    GLuint diffuse_texture_uniform_;

    GLuint obj_velocity_uniform_;
};
