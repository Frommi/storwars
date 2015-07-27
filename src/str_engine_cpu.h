#pragma once

#include "str_engine.h"

class STREngineCPU : public STREngine {
public:
    STREngineCPU(int ticks_per_sec, float map_diameter, int skips_num, int event_cnt, int body_cnt);
    ~STREngineCPU() = default;

    virtual void initEngine();

    virtual STRBody* addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, float eye_IFR_time,
                                const glm::quat& eye_direction, const glm::vec3& eye_force);

    virtual void updateEye(STRBody* body, const glm::quat& eye_direction, const glm::vec3& eye_force);

    virtual void removeSTRBody(STRBody* body);


    virtual void findCollisions();

    virtual void computeForces();
    virtual void computeSTREvents();
    virtual void computeSeenSTREvents(const STREvent& observer);

protected:
    virtual void addVertexToInit(const VertexInfoToInit& vertex);
    virtual void addBodyToInit(const BodyInfoToInit& body);
    virtual void initVertices();
    virtual void initBodies();

    const int kTicksPerSec;
    const float kDt;
    const float kMapDiameter;
    const int kSkipsNum;
    const int kMaxSTREventCnt;
    const int kMaxSTRBodiesCnt;
    
    int tick;

// VRAM "simulating"
    std::vector<STRTrajectory> trajectory_buffer_;
    std::vector<VertexInfo> vertex_info_buffer_;
    std::vector<BodyInfo> body_info_buffer_;
    std::vector<VertexInfoToInit> vertices_to_init_buffer_;
    std::vector<BodyInfoToInit> bodies_to_init_buffer_;
    std::vector<STREvent> seen_events_buffer_;
};
