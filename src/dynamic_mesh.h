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

struct DynamicVertex {
    glm::vec3 position;
    glm::vec2 UV_position;
    glm::vec3 normal;
    uint nodeID;

    DynamicVertex(const glm::vec3& pos, const glm::vec2& UV, const glm::vec3& norm) {
        position    = pos;
        UV_position = UV;
        normal      = norm;
        nodeID      = 0;
    }
};


struct Eye {
    int homo_index;
    int vertex_index;
};


struct DynamicNode {
    glm::vec3 position;
    glm::uvec4 edges;
};


class DynamicMesh {
public:
    DynamicMesh() {}

    bool loadFromFile(const std::string& file_path, const std::string& file_name);

    void render() const;

private:
    void loadMaterials(const std::string& file_path, const aiScene* scene);
    void loadVetices(const aiMesh* ai_mesh, int mesh_index);
    void loadFaces(const aiMesh* ai_mesh, int mesh_index);
    void initBuffers();

    void loadBones(const aiMesh* ai_mesh, int mesh_index);

    struct HomogeneousDynamicMesh {
        std::vector<DynamicVertex> vertices;
        std::vector<glm::uvec3> indices;
        std::vector<DynamicNode> nodes;

        int material_index;

        GLuint VBO;
        GLuint IBO;

        friend class STRBody;
    };

    friend class STRBody;

    std::vector<HomogeneousDynamicMesh> homo_meshes_;
    std::vector<Texture> textures_;
    Eye eye_;
};
