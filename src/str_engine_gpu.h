#pragma once

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <cstdint>

#include "str_event.h"
#include "static_mesh.h"

#define VERTEX_ACTIVE 1 << 0;

struct VertexInfo {
    VertexInfo():
        flags(0),
        body_index(0),
        last_event(),
        force(0.0f),
        mesh_position(0.0f)
    {}

    uint32_t flags;
    uint32_t body_index;
    STREvent last_event;
    glm::vec3 force;
    glm::vec3 mesh_position;
};


struct BodyInfo {
    BodyInfo():
        flags(0),
        eye_index(0),
        eye_rotation(1.0f)
    {}

    uint32_t flags;
    uint32_t eye_index;
    glm::mat4 eye_rotation;
};


#define INVALID_INDEX 0xFFFFFFFF

struct VertexInfoToInit {
    VertexInfoToInit():
        vertex_info(),
        where_to_init(INVALID_INDEX)
    {}

    VertexInfo vertex_info;
    uint32_t where_to_init;
};

struct BodyInfoToInit {
    BodyInfoToInit():
        body_info(),
        where_to_init(INVALID_INDEX)
    {}

    BodyInfo body_info;
    uint32_t where_to_init;
};


class STRBody {
public:
    #define INVALID_EYE_INDEX -1

    STRBody():
        mesh_(nullptr),
        eye_direction_(0.0f, 0.0f, 0.0f, 1.0f),
        eye_force_(0.0f),
        frame_indices_(),
        eye_frame_index_(INVALID_EYE_INDEX),
        body_info_index_(0)
    {}

    void initSTRBody(const StaticMesh& init_mesh) {
        int vertices_cnt = 0;
        for (int i = 0; i < static_cast<int>(init_mesh.homo_meshes_.size()); ++i) {
            if (init_mesh.eye_.homo_index == i)
                eye_frame_index_ = vertices_cnt + init_mesh.eye_.vertex_index;

            vertices_cnt += static_cast<int>(init_mesh.homo_meshes_[i].vertices.size());
        }

        if (eye_frame_index_ == INVALID_EYE_INDEX) {
            fprintf(stderr, "eye not found (%d, %d)", init_mesh.eye_.homo_index, init_mesh.eye_.vertex_index);
            exit(EXIT_FAILURE);
        }

        frame_indices_.resize(vertices_cnt);
    }

    inline void set_eye_direction(const glm::quat& eye_direction) { eye_direction_ = eye_direction; }
    inline void set_eye_force(const glm::vec3& eye_force) { eye_force_ = eye_force; };

private:
    const StaticMesh* mesh_;
    glm::quat eye_direction_;
    glm::vec3 eye_force_;

    std::vector<int> frame_indices_;
    int eye_frame_index_;
    int body_info_index_;

    friend class STREngineGPU;
};


class STREngineGPU : public class STREngine {
	bla bla bla
};
