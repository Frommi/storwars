#pragma once

#include "str_engine.h"

class STREngineCPU : public STREngine {
public:
    STREngineCPU();
    ~STREngineCPU() = default;

    virtual void initEngine(float dt, float map_diameter, int skips_num, int event_cnt, int body_cnt);

    virtual STRBody* addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, 
                                const glm::quat& eye_direction, const glm::vec3& eye_force);

    virtual void updateEye(STRBody* body, const glm::quat& eye_direction, const glm::vec3& eye_force);

    virtual void removeSTRBody(STRBody* body);


    virtual void findCollisions();

    virtual void computeForces();
    virtual void computeSTREvents();
    virtual void computeSeenSTREvents(const STREvent& observer);

private:
    virtual void addVertexToInit(int index, const VertexInfoToInit& vertex);
    virtual void initVertices();
    virtual void initBodies();

    const int kTicksPerSec;
    const float kDt;
    const float kMapDiameter;
    const int kSkipsNum;
    const int kMaxSTREventCnt;
    const int kMaxSTRBodiesCnt;

// VRAM "simulating"
    std::vector<STRTrajectory> trajectory_buffer_;
    std::vector<VertexInfo> vertex_info_buffer_;
    std::vector<BodyInfo> body_info_buffer_;
    std::vector<VertexInfoToInit> vertices_to_init_buffer;
    std::vector<BodyInfoToInit> bodies_to_init_buffer;
};
