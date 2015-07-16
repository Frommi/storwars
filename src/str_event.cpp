#include "str_event.h"

#include <math.h>
#include <cassert>

STREvent::STREvent(glm::vec3 pos, glm::vec3 imp, float i_time, float s_time) {
    position  = pos;
    impulse   = imp;
    IFR_time  = i_time;
    self_time = s_time;
}

bool STREvent::isInPast(const STREvent& event) const {
    float distance = glm::distance(position, event.position);
    float time_difference = IFR_time - event.IFR_time;

    return (time_difference >= 0 && distance <= time_difference);
}


STRTrajectory::STRTrajectory(size_t map_diameter, size_t ticks_per_sec): kSize(map_diameter * ticks_per_sec) {
    trajectory_ = (STREvent*)(malloc(kSize * sizeof(STREvent)));
    first_ = -1;
}

void STRTrajectory::addSTREvent(const STREvent& event) {
    first_ = (first_ + 1) % kSize;
    trajectory_[first_] = event;
}

STREvent STRTrajectory::get_seen_event(const STREvent& observer) const {
    int l = (first_ + 1) % kSize;
    int r = l + kSize;

    while (r - l > 1) {
        int m = (r + l) / 2;
        if (observer.isInPast(trajectory_[m % kSize]))
            l = m;
        else
            r = m;
    }

    STREvent& in_past   = trajectory_[l % kSize];
    STREvent& in_future = trajectory_[r % kSize];

    glm::vec4 f = glm::vec4(in_future.position, in_future.IFR_time);
    glm::vec4 p = glm::vec4(in_past.position, in_past.IFR_time);
    glm::vec4 q = glm::vec4(observer.position, observer.IFR_time);

    glm::vec4 s = q - p;
    glm::vec4 dp = f - p;

    float a = -glm::dot(dp, dp) + 2 * dp.w * dp.w;
    float b = 2 * glm::dot(dp, s) - 4 * dp.w * s.w;
    float c = -glm::dot(s, s) + 2 * s.w * s.w;

    float D = sqrt(b * b - 4 * a * c); // it should always be zero
    assert(fabs(D) < 0.001);

    float alpha = -b / (2 * a); // "$a" can only be zero when object is moving with a speed of light

    STREvent res = STREvent(
       glm::vec3(p) + alpha * glm::vec3(dp),
       in_past.impulse,
       in_past.IFR_time + alpha * (in_future.IFR_time - in_past.IFR_time),
       in_past.self_time + alpha * (in_future.self_time - in_past.self_time)
    );

    return res;
}

STRTrajectory::~STRTrajectory() {
    free(trajectory_);
}
