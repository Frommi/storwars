#pragma once

#include <glm/glm.hpp>

struct STREvent {
    STREvent(glm::vec3 pos, glm::vec3 imp, float i_time, float s_time);

    bool isInPast(const STREvent& event) const;

    glm::vec3 position;
    glm::vec3 impulse;
    float IFR_time;
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
