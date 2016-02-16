#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE_XYZW
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
        position     = pos;
        UV_position  = UV;
        normal       = norm;
    }
};


class StaticMesh {
public:
    StaticMesh() {}

    bool loadFromFile(const std::string& file_path, const std::string& file_name);

    void render() const;

private:
    void loadMaterials(const std::string& file_path, const aiScene* scene);
    void loadVetices(const aiMesh* ai_mesh, int mesh_index);
    void loadFaces(const aiMesh* ai_mesh, int mesh_index);
    void initBuffers();

    struct HomogeneousStaticMesh {
        std::vector<StaticVertex> vertices;
        std::vector<glm::uvec3> indices;

	    int material_index;

        GLuint VBO;
        GLuint IBO;
    };

    std::vector<HomogeneousStaticMesh> homo_meshes_;
    std::vector<Texture> textures_;
};
