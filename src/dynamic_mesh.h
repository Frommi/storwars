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
    glm::uvec4 boneIDs;
    glm::vec4 bone_weights;

    DynamicVertex(const glm::vec3& pos, const glm::vec2& UV, const glm::vec3& norm) {
        position     = pos;
        UV_position  = UV;
        normal       = norm;
        boneIDs      = glm::uvec4(0);
        bone_weights = glm::vec4(0.0f);
    }
};


struct Eye {
    int homo_index;
    int vertex_index;
};


class DynamicMesh {
public:
    DynamicMesh() {}

    bool loadFromFile(const std::string& file_path, const std::string& file_name);

    void render() const;

    float eye_dist() const {
        return glm::length(homo_meshes_[eye_.homo_index].vertices[eye_.vertex_index].position);
    }

private:
    void loadMaterials(const std::string& file_path, const aiScene* scene);
    void loadVetices(const aiMesh* ai_mesh, int mesh_index);
    void loadFaces(const aiMesh* ai_mesh, int mesh_index);
    void initBuffers();

    void loadBones(const aiMesh* ai_mesh, int mesh_index);

    struct HomogeneousDynamicMesh {
        std::vector<DynamicVertex> vertices;
        std::vector<glm::uvec3> indices;

        int material_index;

        GLuint VBO;
        GLuint IBO;

        friend class STRBody;
    };

    void findEye();

    friend class STRBody;

    std::vector<HomogeneousStaticMesh> homo_meshes_;
    std::vector<Texture> textures_;
    Eye eye_;
};
