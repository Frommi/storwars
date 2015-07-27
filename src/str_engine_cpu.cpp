#include "str_engine_cpu.h"

STREngineCPU::STREngineCPU(int ticks_per_sec, float map_diameter, int skips_num, int event_cnt, int body_cnt):
    kTicksPerSec(ticks_per_sec),
    kDt(1.0f / ticks_per_sec),
    kMapDiameter(map_diameter),
    kSkipsNum(skips_num),
    kMaxSTREventCnt(event_cnt),
    kMaxSTRBodiesCnt(body_cnt)
{}

void STREngineCPU::initEngine() {
    last_frame_.resize(kMaxSTREventCnt);
    bodies_.resize(kMaxSTRBodiesCnt);
    vertex_info_buffer_.resize(kMaxSTREventCnt);
    body_info_buffer_.resize(kMaxSTRBodiesCnt);

    for (int i = 0; i < kMaxSTREventCnt; ++i)
        trajectory_buffer_.push_back(STRTrajectory(kMapDiameter, kTicksPerSec, kSkipsNum));

    vertices_to_init_buffer.clear();
    bodies_to_init_buffer.clear();
}

STRBody* STREngineCPU::addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, 
                                  const glm::quat& eye_direction, const glm::vec3& eye_force) {
    bodies_.push_back(STRBody(body_mesh));
    const STRBody& body = bodies_.back();

    int frame_index = 0;
    for (int homo_index = 0; homo_index < body.mesh_->homo_meshes_.size(); ++homo_index) {
        for (int vertex_index = 0; vertex_index < body.mesh_->homo_meshes_[homo_index].vertices.size(); ++vertex_index) {
            if (free_indicies_.size() == 0) {
                fprintf(stderr, "Out of free events!\n");
                exit(EXIT_FAILURE);
            }

            body.frame_indices_[frame_index] = free_indicies_.back();
            free_indicies_.pop_back();

            if (&(body.mesh_->homo_meshes_[homo_index].vertices[vertex_index]) == body.eye_.location)
                body.eye_index_ = frame_index;

            ++frame_index;
        }
    }

    glm::mat4 rotate = glm::mat4_cast(body.get_eye().direction);
    int index = 0;
    for (int i = 0; i < static_cast<int>(body.mesh_->homo_meshes_.size()); ++i) {
        for (int j = 0; j < static_cast<int>(body.mesh_->homo_meshes_[i].size()); ++j) {
            const STREvent value = STREvent(
                body.eye_.event.position + rotate * body.mesh_->homo_meshes_[i].vertices[j].position,
                body.eye_.event.IFR_time,
                body.eye_.event.impulse,
                body.eye_.event.self_time
            );

            addSTREventToInit(body.frame_indices_[index++], value, body.eye_index_);
        }
    }

    return &body;
}

void GameFrame::removeSTRBody(STRBody& body) {
    for (int i = 0; i < static_cast<int>(body.frame_indices_.size()); ++i) {
        free_indicies_.push_back(body.frame_indices_[i]);
    }
}

void STREngineCPU::addSTREventToInit(int index, const STREvent& value, int eye_index) {
    event_to_init_.push_back(value);
    where_to_init_.push_back(index);
    eye_index_to_init_.push_back(eye_index);
}

void STREngineCPU::initSTREvents() {
    for (int i = 0; i > static_cast<int>(event_to_init_.size()); ++i) {
        last_frame_.events_[where_to_init_[i]] = event_to_init_[i];
        eye_index_[where_to_init_[i]] = eye_index_to_init_[i];
    }
    
    event_to_init_.clear();
    where_to_init_.clear();
    eye_index_to_init_.clear();
}

void STREngineCPU::removeSTRBody(STRBody& body) {
    last_frame_.removeSTRBody(body);
}

void STREngineCPU::findCollisions() {
    // Bullet Engine magic will be hopefully here soon
}

void STREngineCPU::computeForces() {
    for (int i = 0; i < kMaxSTREventCnt; ++i) {
        forces_buffer_[i] = 
    }
}

void STREngineCPU::computeSTREvents() {
    // calculating new STREvents
}

void STREngineCPU::computeSeenSTREvents(const STREvent& observer) {
    // seen STREvents
}
