#pragma once

#include <cstdlib>
#include <unordered_map>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

#include "static_mesh.h"

struct VoxelVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class VoxelGenerator {
public:
    VoxelGenerator();
    ~VoxelGenerator() = default;

    StaticMesh* generateMesh(const glm::ivec3& resolution, float (*density)(const glm::vec3& pos)) const;

private:


};
