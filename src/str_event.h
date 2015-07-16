#pragma once

#include <glm/glm.hpp>

struct STREvent {
    STREvent();
    STREvent(const glm::vec3& pos, float i_time, const glm::vec3& imp, float s_time);

    bool isInPast(const STREvent& event) const;

    glm::vec3 position;
    float IFR_time;
    glm::vec3 impulse;
    float self_time;
};

class STRTrajectory {
public:
    STRTrajectory(size_t map_diameter, size_t ticks_per_sec);

    void addSTREvent(const STREvent& event);

//    STREvent get_event(float ifr_time);
    STREvent get_seen_event(const STREvent& observer) const;

    ~STRTrajectory();

private:
    STREvent* trajectory_;
    int first_;
    const size_t kSize;
};
