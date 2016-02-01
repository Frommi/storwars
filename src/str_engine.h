#pragma once

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <cstdint>

#include "str_event.h"
#include "static_mesh.h"

struct STRBody {
	STRBody(const Static_Mesh* mesh);

    const StaticMesh* mesh_;
    glm::quat eye_direction_;
    glm::vec3 eye_force_;
};


class STREngine {
public:
    virtual void initEngine() = 0;

    virtual STRBody* addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, float eye_IFR_time,
                                const glm::quat& eye_direction, const glm::vec3& eye_force) = 0;

    virtual void updateEye(STRBody* body, const glm::quat& eye_direction, const glm::vec3& eye_force) = 0;

    virtual void removeSTRBody(STRBody* body) = 0;


    virtual void findCollisions() = 0;

    virtual void computeForces() = 0;
    virtual void computeSTREvents() = 0;
    virtual void computeSeenSTREvents(const STREvent& observer) = 0;

protected:
    virtual void addVertexToInit(const VertexInfoToInit& vertex) = 0;
    virtual void addBodyToInit(const BodyInfoToInit& body) = 0;
    virtual void initVertices() = 0;
    virtual void initBodies() = 0;

    std::vector<int> free_vertex_indicies_;
    std::vector<int> free_body_indices_;

    std::vector<STRBody> bodies_;
    std::vector<VertexInfo> last_vertex_frame_;
    std::vector<BodyInfo> last_body_frame_;

    std::vector<VertexInfoToInit> vertices_to_init_;
    std::vector<BodyInfoToInit> bodies_to_init_;
};
