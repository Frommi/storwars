#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	virtual bool initShaderProgram();

protected:
	bool addShader(const std::string& file_name, const GLenum shader_type);
	bool compileShaderProgram();

private:
	static std::string loadShaderFromFile(const std::string& file_name);

	GLuint shader_program_obj_;
	std::vector<GLuint> shader_objects_;
};
