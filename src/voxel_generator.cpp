#include "voxel_generator.h"

VoxelGenerator::VoxelGenerator() {

}

int getIndex(const glm::ivec3 resolution, const glm::ivec3& x) {
    return x.x * resolution.y * resolution.z + x.y * resolution.z + x.z;
}

StaticMesh* VoxelGenerator::generateMesh(const glm::ivec3& resolution, float (*density)(const glm::vec3& pos)) const {
    const float step = 0.001f;
    const glm::vec3 inverse_resolution = glm::vec3(1.0f) / glm::vec3(resolution);

    glm::ivec3 edge_point1, edge_point2, edge_offset;

    StaticMesh* mesh = new StaticMesh();
    
    mesh->textures_.resize(1);
    if (!mesh->textures_[0].loadFromFile("models/island/Maps/rc1.jpg")) {
        fprintf(stderr, "Failed loading texture models/island/Maps/rc1.jpg in VoxelGenerator\n");
        exit(1);
    }

    mesh->homo_meshes_.resize(1);
    mesh->homo_meshes_[0].material_index = 0;

    std::vector<StaticVertex>& vertices = mesh->vertices_;
    std::vector<glm::uvec3>& indices = mesh->homo_meshes_[0].indices;

    std::unordered_map<int, int> vertex_map;
    std::unordered_map<int, int> cnt;

    for (edge_point1.x = 0; edge_point1.x < resolution.x; ++edge_point1.x) {
        for (edge_point1.y = 0; edge_point1.y < resolution.y; ++edge_point1.y) {
            for (edge_point1.z = 0; edge_point1.z < resolution.z; ++edge_point1.z) {
                for (int i = 0; i < 3; ++i) {
                    const glm::ivec3 edge_offset = glm::ivec3(i == 0, i == 1, i == 2);
                    const glm::ivec3 edge_point2 = edge_point1 + edge_offset;

                    glm::vec3 p1 = glm::vec3(edge_point1) * inverse_resolution;
                    glm::vec3 p2 = glm::vec3(edge_point2) * inverse_resolution;

                    float d1 = density(p1);
                    float d2 = density(p2);

                    if (d1 * d2 <= 0.0f) {
                        for (int c = 0; c < 20; ++c) {
                            //float alpha = d2 / (d2 - d1);
                            float alpha = 0.5;
                            glm::vec3 p = p1 * (1.0f - alpha) + p2 * alpha;
                            
                            float d0 = density(p);
                            if (d0 * d1 <= 0.0f) {
                                d2 = d0;
                                p2 = p;
                            } else {
                                d1 = d0;
                                p1 = p;
                            }
                        }

                        //float alpha = d2 / (d2 - d1);
                        float alpha = 0.5;
                        glm::vec3 p = p1 * (1.0f - alpha) + p2 * alpha;
                        
                        //printf("%f (%f, %f, %f)\n", density(p), d1, d2, alpha);
                        
                        int k = 0;
                        glm::uvec4 quad = glm::uvec4(0);
                        for (int dx = -(i != 0); dx <= 0; ++dx) {
                            for (int dy = -(i != 1); dy <= 0; ++dy) {
                                for (int dz = -(i != 2); dz <= 0; ++dz) {
                                    const glm::ivec3 cube_pos = edge_point1 + glm::ivec3(dx, dy, dz);
                                    const int id = getIndex(resolution, cube_pos);

                                    if (!vertex_map.count(id)) {
                                        vertex_map[id] = vertices.size();
                                        cnt[id] = 0;
                                        vertices.push_back(StaticVertex(glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)));
                                    }

                                    int vertex_id = vertex_map[id];
                                    vertices[vertex_id].position += p;
                                    ++cnt[id];

                                    quad[k++] = vertex_id;
                                }
                            }
                        }
                        indices.push_back(glm::uvec3(quad.x, quad.y, quad.z));
                        indices.push_back(glm::uvec3(quad.y, quad.w, quad.z));

                        //indices.push_back(glm::uvec3(quad.w, quad.z, quad.x));
                        //indices.push_back(glm::uvec3(quad.y, quad.w, quad.x));
                    }
                }
            }
        }
    }

    glm::ivec3 cube = glm::ivec3(0);
    for (cube.x = 0; cube.x < resolution.x; ++cube.x) {
        for (cube.y = 0; cube.y < resolution.y; ++cube.y) {
            for (cube.z = 0; cube.z < resolution.z; ++cube.z) {
                const int id = getIndex(resolution, cube);
                if (vertex_map.count(id)) {
                    StaticVertex& v = vertices[vertex_map[id]];
                    v.position /= cnt[id];
                    v.normal = glm::vec3(
                        density(v.position + glm::vec3(step, 0.0f, 0.0f)) - density(v.position + glm::vec3(-step, 0.0f, 0.0f)),
                        density(v.position + glm::vec3(0.0f, step, 0.0f)) - density(v.position + glm::vec3(0.0f, -step, 0.0f)),
                        density(v.position + glm::vec3(0.0f, 0.0f, step)) - density(v.position + glm::vec3(0.0f, 0.0f, -step))
                    );
                    //fprintf(stderr, "normal = %f %f %f\n", v.normal.x, v.normal.y, v.normal.z);

                    v.normal = glm::normalize(v.normal);
                    //fprintf(stderr, "norm normal = %f %f %f\n", v.normal.x, v.normal.y, v.normal.z);
                    v.UV_position = glm::vec2(v.position.x / resolution.x, v.position.y / resolution.y);

                    //v.position -= glm::vec3(0.5f);
                    v.position *= 100.0f;

                    //fprintf(stderr, "n= %f %f\n", v.UV_position.x, v.UV_position.y);
                    //fprintf(stderr, "pos = %f %f %f\n", v.position.x, v.position.y, v.position.z);
                }
            }
        }
    }

    mesh->homo_meshes_[0].vertex_offset = 0;
    mesh->homo_meshes_[0].vertex_number = vertices.size();

    fprintf(stderr, "%d\n", vertices.size());
    fprintf(stderr, "%d\n", indices.size());

    mesh->initBuffers();

    return mesh;
}
