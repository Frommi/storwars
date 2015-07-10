#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

struct StaticVertex {
    glm::vec3 position;
    glm::vec2 UV_position;
    glm::vec3 normal;
};

class StaticMesh {
public:
    StaticMesh() {}

    void loadFromFile(const std::string& file_name);

    void Render() const;

private:
    struct HomogeneousStaticMesh {
        std::vector<StaticVertex> vertices;
        std::vector<glm::uvec3> indices;

        GLuint VBO;
        GLuint VBI;
    };

    std::vector<HomogeneousStaticMesh> homo_meshs;
};
