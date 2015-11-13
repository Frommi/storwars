#pragma once

#include "include/SOIL.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Texture {
public:
	Texture() {}
	~Texture();

	bool loadFromFile(const std::string& file_name);

	void bindToUnit(GLenum texture_unit) const;

private:
	GLuint texture_object_;

	int width_;
	int height_;

	unsigned char* image_data_;
};

