#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "texture.h"

#include <vector>
#include <string>

struct StaticVertex {
    glm::vec3 position;
    glm::vec2 UV_position;
    glm::vec3 normal;

    StaticVertex(const glm::vec3& pos, const glm::vec2& UV, const glm::vec3& norm) {
        position    = pos;
        UV_position = UV;
        normal      = norm;
    }
};


struct Eye {
    int homo_index;
    int vertex_index;
};


class StaticMesh {
public:
    StaticMesh() {}

    bool loadFromFile(const std::string& file_name);

    void render() const;

private:
    struct HomogeneousStaticMesh {
        std::vector<StaticVertex> vertices;
        std::vector<glm::uvec3> indices;

	    int material_index_;

        GLuint VBO;
        GLuint IBO;

        friend struct STRBody;
        friend class STREngineCPU;
    };

    void findEye();

    friend struct STRBody;
    friend class STREngineCPU;

    std::vector<HomogeneousStaticMesh> homo_meshes_;
    std::vector<Texture> textures_;
    Eye eye_;
};
