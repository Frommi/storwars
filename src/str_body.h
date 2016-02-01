#pragma once

#include "static_mesh.h"
#include "str_event.h"

class STRVertex {
public:
    STRVertex() :
        mesh_homo_index_(-1),
        mesh_vertex_index_(-1)
    {}

    void initVertex(const STREvent& event, int mesh_homo_index, int mesh_vertex_index);

    void setForce(const glm::vec3& force);
    void addForce(const glm::vec3& force);
    void addString(Vertex* end, float tension);

    ~STRVertex() = default;

private:
    friend class STRBody;

    struct STRString {
        Vertex* end_vertex;
        glm::vec3 direction;
        float tension;
    };

    STRTrajectory trajectory_;
    std::vector<STRString> strings_;
    glm::vec3 force_;

    int mesh_homo_index_;
    int mesh_vertex_index_;
};


class STRBody {
public:
    STRBody(const StaticMesh* body_mesh);

    void initBody(const STREye& eye);

    void computeForces();
    void tick(float dt);

    void render() const;
    
private:
    void prepareStrings();

    void addStringForces();
    void addEyeForces();

    std::vector< std::vector<STRVertex> > vertices_;

    STRVertex* eye_;
    glm::quat sight_direction_;
    
    StaticMesh* body_mesh_;
};
