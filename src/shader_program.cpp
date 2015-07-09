#include "shader_program.h"

ShaderProgram::ShaderProgram() {
	shader_program_obj_ = 0;
}

ShaderProgram::~ShaderProgram() {
	for (int i = 0; i < shader_objects_.size(); ++i)
		glDeleteShader(shader_objects_[i]);

	if (shader_program_obj_ != 0) {
		glDeleteProgram(shader_program_obj_);
		shader_program_obj_ = 0;
	}
}

bool ShaderProgram::initShaderProgram() {
	shader_program_obj_ = glCreateProgram();

	if (shader_program_obj_ == 0) {
		fprintf(stderr, "Error creating shader program\n");
        return false;
	}

	return true;
}

std::string ShaderProgram::loadShaderFromFile(const std::string& file_name) {
	std::string data;
	if (FILE *fp = fopen(file_name.c_str(), "r")) {
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			data.insert(data.end(), buf, buf + len);
		fclose(fp);
	}

	return data;
}

bool ShaderProgram::addShader(const std::string& file_name, const GLenum shader_type) {
	const std::string shader_text = loadShaderFromFile(file_name);

    GLuint shader_obj = glCreateShader(shader_type);
    if (shader_obj == 0){
        fprintf(stderr, "Error creating shader type %d\n", shader_type);
        return false;
    }

    shader_objects_.push_back(shader_obj);

    const GLchar* p[1];
    p[0] = shader_text.c_str();
    GLint Lengths[1];
    Lengths[0] = shader_text.size();
    glShaderSource(shader_obj, 1, p, Lengths);

    glCompileShader(shader_obj);

    GLint success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);

    if (!success){
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader_type, info_log);
        return false;
    }

    glAttachShader(shader_program_obj_, shader_obj);

    return true;
}

bool ShaderProgram::compileShaderProgram() {
	GLint success = 0;
    GLchar error_log[1024] = { 0 };

    glLinkProgram(shader_program_obj_);

    glGetProgramiv(shader_program_obj_, GL_LINK_STATUS, &success);
    if (success == 0){
        glGetProgramInfoLog(shader_program_obj_, sizeof(error_log), NULL, error_log);
        fprintf(stderr, "Error linking shader program: '%s'\n", error_log);
        return false;
    }

    glValidateProgram(shader_program_obj_);
    glGetProgramiv(shader_program_obj_, GL_VALIDATE_STATUS, &success);
    if (success == 0){
        glGetProgramInfoLog(shader_program_obj_, sizeof(error_log), NULL, error_log);
        fprintf(stderr, "Invalid shader program: '%s'\n", error_log);
        return false;
    }

    for (int i = 0; i < shader_objects_.size(); ++i)
		glDeleteShader(shader_objects_[i]);
    shader_objects_.clear();

    return true;
}
