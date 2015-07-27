#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <cstdint>

#include "str_event.h"
#include "static_mesh.h"

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
        mesh_(NULL),
        eye_direction_(0.0f, 0.0f, 0.0f, 1.0f),
        eye_force_(0.0f),
        frame_indices_(),
        eye_frame_index_(INVALID_EYE_INDEX)
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

    friend class STREngine;
    friend class STREngineCPU;
};


class STREngine {
public:
    virtual void initEngine(float dt, float map_diameter, int skips_num, int event_cnt, int body_cnt) = 0;

    virtual STRBody* addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, 
                                const glm::quat& eye_direction, const glm::vec3& eye_force) = 0;

    virtual void updateEye(STRBody* body, const glm::quat& eye_direction, const glm::vec3& eye_force) = 0;

    virtual void removeSTRBody(STRBody* body) = 0;


    virtual void findCollisions() = 0;

    virtual void computeForces() = 0;
    virtual void computeSTREvents() = 0;
    virtual void computeSeenSTREvents(const STREvent& observer) = 0;

private:
    virtual void addVertexToInit(int index, const VertexInfoToInit& vertex) = 0;
    virtual void initVertices() = 0;
    virtual void initBodies() = 0;

    std::vector<STRBody> bodies_;
    std::vector<VertexInfo> last_frame_;

    std::vector<VertexInfoToInit> vertices_to_init_;
    std::vector<BodyInfoToInit> bodies_to_init_;
};
