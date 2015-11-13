#include "texture.h"
                                  
Texture::~Texture() {
	if (image_data_ != NULL)
		SOIL_free_image_data(image_data_);
}

bool Texture::loadFromFile(const std::string& file_name) {
    glGenTextures(1, &texture_object_);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_object_);

    image_data_ = SOIL_load_image(file_name.c_str(), &width_, &height_, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data_);

	if(texture_object_ == 0) {
		fprintf(stderr, "SOIL loading error: '%s'\n", SOIL_last_result());
		return 0;
	}

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}

void Texture::bindToUnit(GLenum texture_unit) const {
	glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_object_);
}
