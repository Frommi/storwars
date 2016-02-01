#include "str_event.h"

#include <cmath>
#include <cassert>

STREvent::STREvent() {
    position  = glm::vec3(0.0f);
    IFR_time  = 0.0f;
    impulse   = glm::vec3(0.0f);
    self_time = 0.0f;
}

STREvent::STREvent(const glm::vec3& position, float IFR_time, const glm::vec3& impulse, float self_time):
    position(position),
    IFR_time(IFR_time),
    impulse(impulse),
    self_time(self_time)
{}

bool STREvent::isInMyPast(const STREvent& event) const {
    float distance = glm::distance(position, event.position);
    float time_difference = IFR_time - event.IFR_time;

    return (time_difference >= 0 && distance <= time_difference);
}

STREvent STREvent::applyForce(const glm::vec3& force, float kDt) const {
    float force_sqr = glm::dot(force, force);
    float g = 1.0f / sqrt(1.0f + glm::dot(impulse, impulse));

    if (force_sqr < 1e-12) {
        double t0 = kDt * g;
        glm::vec3 v = impulse * g;
        return STREvent(position + v * kDt, IFR_time + kDt, impulse, self_time + t0);
    }

    float inverse_force_abs = 1.0f / sqrt(force_sqr);
    float inverse_force_sqr = 1.0f / force_sqr;

    glm::vec3 at = force * kDt;
    glm::vec3 new_impulse = impulse + at;
    float new_g = sqrt(1.0f + glm::dot(new_impulse, new_impulse));

    float impulse_proj = glm::dot(impulse, force) * inverse_force_abs;

    float t0 = log((new_g + glm::length(at) + impulse_proj) / (g + impulse_proj)) * inverse_force_abs;

    glm::vec3 inverse_force = force * inverse_force_sqr;
    glm::vec3 force_impulse_force = glm::cross(force, (glm::cross(impulse, force))) * (t0 * inverse_force_sqr);

    glm::vec3 new_position = position + (inverse_force * (new_g - g)) + force_impulse_force;

    return STREvent(new_position, IFR_time + kDt, new_impulse, self_time + t0);
}


STRTrajectory::STRTrajectory(int queue_length):
    queue_length_(queue_length),
    add_mask_(1)
{
    data_ = std::vector< std::deque<STREvent> >();
}

void STRTrajectory::initTrajectory(const STREvent& event) {
    start_event_ = event;
    last_event_ = event;
    addSTREvent(event);
}

void STRTrajectory::pushToQueue(int i, const STREvent& event) {
    while (static_cast<int>(data_.size()) <= i) {
        data_.push_back(std::deque<STREvent>());
        data_.back().push_back(start_event_);
    }
    
    data_[i].push_back(event);
    if (static_cast<int>(data_[i].size()) > queue_length_) 
        data_[i].pop_front();
}

void STRTrajectory::addSTREvent(const STREvent& event) {
    last_event_ = event;

    int i = 0;
    int mask = add_mask_;
    while ((mask & 1) == 0) {
        pushToQueue(i, event);
        mask >>= 1;
        ++i;
    }
    pushToQueue(i, event);
}

STREvent STRTrajectory::getLastSTREvent() const {
    return last_event_;
}

bool STRTrajectory::isSeen(const STREvent& observer) const {
    return observer.isInMyPast(start_event_);
}

STREvent STRTrajectory::getSeenSTREvent(const STREvent& observer) const {
    assert(isSeen(observer));

    int l = 0;
    int r = data_.size();
    while (r - l > 1) {
        int m = (l + r) >> 1;
        if (observer.isInMyPast(data_[m].front()))
            l = m;
        else
            r = m;
    }
    int i = l;
    
    l = 0;
    r = data_[i].size();
    while (r - l > 1) {
        int m = (l + r) >> 1;
        if (observer.isInMyPast(data_[i][m]))
            l = m;
        else
            r = m;
    }

    STREvent in_past = data_[i][l];
    return in_past;
}

/*
STREvent STRTrajectory::get_seen_event(const STREvent& observer) const {
    int l = (first_ + 1) % kSize;
    int r = l + kSize;

    while (r - l > 1) {
        int m = (r + l) / 2;
        if (observer.isInMyPast(trajectory_[m % kSize]))
            l = m;
        else
            r = m;
    }

    STREvent& in_past   = trajectory_[l % kSize];
    return in_past;

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
    assert(fabs(D) < 1e-5);

    float alpha = -b / (2 * a); // "$a" can only be zero when object is moving with a speed of light

    STREvent res = STREvent(
       glm::vec3(p) + alpha * glm::vec3(dp),
       in_past.IFR_time + alpha * (in_future.IFR_time - in_past.IFR_time),
       in_past.impulse + alpha * (in_future.impulse - in_past.impulse),
       in_past.self_time + alpha * (in_future.self_time - in_past.self_time)
    );

    return res;
}
*/
