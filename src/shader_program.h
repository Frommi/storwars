#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
#include <vector>

#define INVALID_LOCATION 0xFFFFFFFF

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    inline void enable() const {
        glUseProgram(shader_program_obj_);
    }

protected:
    bool preInit();
    bool addShader(const std::string& file_name, const GLenum shader_type);
    bool compileShaderProgram();

    inline GLint getUniformLocation(const std::string& uniform_name) {
        GLint location = glGetUniformLocation(shader_program_obj_, uniform_name.c_str());
        if (static_cast<unsigned int>(location) == INVALID_LOCATION)
            fprintf(stderr, "Unable to get the location of uniform '%s'\n", uniform_name.c_str());

        return location;
    }

private:
    static std::string loadShaderFromFile(const std::string& file_name);

    GLuint shader_program_obj_;
    std::vector<GLuint> shader_objects_;
};
