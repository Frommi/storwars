#pragma once

#include <cstdlib>
#include <vector>
#include <deque>

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

struct STREvent {
    STREvent();
    STREvent(const glm::vec3& position, float IFR_time, const glm::vec3& impulse, float self_time);

    ~STREvent() = default;

    bool isInMyPast(const STREvent& event) const;
    STREvent applyForce(const glm::vec3& force, float kDt=1.0/60.0) const;

    glm::vec3 position;
    float IFR_time;
    glm::vec3 impulse;
    float self_time;
};

class STRTrajectory {
public:
    STRTrajectory(int queue_length=16);

    ~STRTrajectory() = default;

    void initTrajectory(const STREvent& event);
    void addSTREvent(const STREvent& event);
    
//    STREvent getSTREvent(float IFR_time) const;
    STREvent getLastSTREvent() const;
    bool isSeen(const STREvent& observer) const;
    STREvent getSeenSTREvent(const STREvent& observer) const;

private:
    void pushToQueue(int i, const STREvent& event);

    std::vector< std::deque<STREvent> > data_;
    STREvent start_event_;
    STREvent last_event_;
    int queue_length_;
    int add_mask_;
};
