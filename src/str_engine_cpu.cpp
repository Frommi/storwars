#include "str_engine_cpu.h"

STREngineCPU::STREngineCPU(int ticks_per_sec, float map_diameter, int skips_num, int event_cnt, int body_cnt):
    kTicksPerSec(ticks_per_sec),
    kDt(1.0f / ticks_per_sec),
    kMapDiameter(map_diameter),
    kSkipsNum(skips_num),
    kMaxSTREventCnt(event_cnt),
    kMaxSTRBodiesCnt(body_cnt),
    tick(0)
{}

void STREngineCPU::initEngine() {
    free_vertex_indicies_.resize(kMaxSTREventCnt);
    for (int i = 0; i < kMaxSTREventCnt; ++i)
        free_vertex_indicies_[i] = i;

    free_body_indices_.resize(kMaxSTRBodiesCnt);
    for (int i = 0; i < kMaxSTRBodiesCnt; ++i)
        free_body_indices_[i] = i;

    last_vertex_frame_.resize(kMaxSTREventCnt);
    bodies_.resize(kMaxSTRBodiesCnt);
    vertex_info_buffer_.resize(kMaxSTREventCnt);
    body_info_buffer_.resize(kMaxSTRBodiesCnt);

    for (int i = 0; i < kMaxSTREventCnt; ++i)
        trajectory_buffer_.push_back(STRTrajectory(kMapDiameter, kTicksPerSec, kSkipsNum));

    vertices_to_init_buffer_.clear();
    bodies_to_init_buffer_.clear();
}

STRBody* STREngineCPU::addSTRBody(const StaticMesh& body_mesh, const glm::vec3& eye_position, float eye_IFR_time, 
                                  const glm::quat& eye_direction, const glm::vec3& eye_force) {
    bodies_.push_back(STRBody());
    bodies_.back().initSTRBody(body_mesh);
    STRBody* body = &(bodies_.back());

    glm::mat4 rotate = glm::mat4_cast(eye_direction);
    body->body_info_index_ = free_body_indices_.back();
    free_body_indices_.pop_back();

    BodyInfoToInit body_info;
    body_info.where_to_init = body->body_info_index_;
    body_info.body_info.flags |= VERTEX_ACTIVE;
    body_info.body_info.eye_index = body->eye_frame_index_;
    body_info.body_info.eye_rotation = rotate;

    addBodyToInit(body_info);

    int frame_index = 0;
    for (int homo_index = 0; homo_index < static_cast<int>(body->mesh_->homo_meshes_.size()); ++homo_index) {
        for (int vertex_index = 0; vertex_index < static_cast<int>(body->mesh_->homo_meshes_[homo_index].vertices.size()); ++vertex_index) {
            if (free_vertex_indicies_.size() == 0) {
                fprintf(stderr, "Out of free events!\n");
                exit(EXIT_FAILURE);
            }

            body->frame_indices_[frame_index] = free_vertex_indicies_.back();
            free_vertex_indicies_.pop_back();

            STREvent event = STREvent(
                eye_position + glm::vec3(rotate * glm::vec4(body->mesh_->homo_meshes_[homo_index].vertices[vertex_index].position, 1.0f)),
                eye_IFR_time,
                glm::vec3(0.0f),
                eye_IFR_time
            );

            VertexInfoToInit vertex;
            vertex.vertex_info.flags |= VERTEX_ACTIVE;
            vertex.vertex_info.body_index = body->body_info_index_;
            vertex.vertex_info.last_event = event;
            vertex.vertex_info.force = eye_force;
            vertex.vertex_info.mesh_position = body->mesh_->homo_meshes_[homo_index].vertices[vertex_index].position;
            vertex.where_to_init = body->frame_indices_[frame_index];

            addVertexToInit(vertex);

            ++frame_index;
        }
    }

    return body;
}

void STREngineCPU::updateEye(STRBody* body, const glm::quat& eye_direction, const glm::vec3& eye_force) {
    body->eye_direction_ = eye_direction;
    body->eye_force_ = eye_force;

    VertexInfo vertex_info = last_vertex_frame_[body->eye_frame_index_];
    vertex_info.force = eye_force;

    BodyInfo body_info = last_body_frame_[vertex_info.body_index];
    body_info.eye_rotation = glm::mat4_cast(eye_direction);

    VertexInfoToInit vertex_to_init;
    vertex_to_init.vertex_info = vertex_info;
    vertex_to_init.where_to_init = body->eye_frame_index_;

    BodyInfoToInit body_to_init;
    body_to_init.body_info = body_info;
    body_to_init.where_to_init = vertex_info.body_index;

    addBodyToInit(body_to_init);
    addVertexToInit(vertex_to_init);
}

void STREngineCPU::removeSTRBody(STRBody* body) {
    for (int i = 0; i < static_cast<int>(body->frame_indices_.size()); ++i)
        free_vertex_indicies_.push_back(body->frame_indices_[i]);

    free_body_indices_.push_back(body->body_info_index_);
    body->frame_indices_.clear();
}

void STREngineCPU::addVertexToInit(const VertexInfoToInit& vertex) {
    vertices_to_init_.push_back(vertex);
}

void STREngineCPU::addBodyToInit(const BodyInfoToInit& body) {
    bodies_to_init_.push_back(body);
}

void STREngineCPU::initVertices() {
    for (int i = 0; i > static_cast<int>(vertices_to_init_.size()); ++i)
        vertex_info_buffer_[vertices_to_init_[i].where_to_init] = vertices_to_init_[i].vertex_info;

    vertices_to_init_.clear();
}

void STREngineCPU::initBodies() {
    for (int i = 0; i > static_cast<int>(bodies_to_init_.size()); ++i)
        body_info_buffer_[bodies_to_init_[i].where_to_init] = bodies_to_init_[i].body_info;

    bodies_to_init_.clear();
}

void STREngineCPU::findCollisions() {
    // Bullet Engine magic will be hopefully here soon
}

void STREngineCPU::computeForces() {
    for (int i = 0; i < kMaxSTREventCnt; ++i) {
        VertexInfo vertex = vertex_info_buffer_[i];
        BodyInfo body = body_info_buffer_[vertex.body_index];

        STREvent seen_eye = trajectory_buffer_[body.eye_index].get_seen_event(vertex.last_event);
        vertex.force = glm::vec3((body.eye_rotation * glm::vec4(vertex.mesh_position, 1.0f))) - 
                       (seen_eye.position - vertex.last_event.position);

        vertex_info_buffer_[i] = vertex;
    }
}

void STREngineCPU::computeSTREvents() {
    for (int i = 0; i < kMaxSTREventCnt; ++i) {
        STREvent event = vertex_info_buffer_[i].last_event;
        glm::vec3 force = vertex_info_buffer_[i].force;

        float force_sqr = glm::dot(force, force);
        float g = 1.0f / sqrt(1.0f + glm::dot(event.impulse, event.impulse));

        if (force_sqr < 1e-12) {
            double t0 = kDt * g;
            glm::vec3 v = event.impulse * g;
            vertex_info_buffer_[i].last_event = STREvent(event.position + v * kDt, event.IFR_time + kDt,
                                                         event.impulse, event.self_time + t0);
            continue;
        }

        float inverse_force_abs = 1.0f / sqrt(force_sqr);
        float inverse_force_sqr = 1.0f / force_sqr;

        glm::vec3 at = force * kDt;
        glm::vec3 new_impulse = event.impulse + at;
        float new_g = sqrt(1.0f + glm::dot(new_impulse, new_impulse));

        float impulse_proj = glm::dot(event.impulse, force) * inverse_force_abs;

        float t0 = log((new_g + glm::length(at) + impulse_proj) / (g + impulse_proj)) * inverse_force_abs;

        glm::vec3 inverse_force = force * inverse_force_sqr;
        glm::vec3 force_impulse_force = glm::cross(force, (glm::cross(event.impulse, force))) * (t0 * inverse_force_sqr);

        glm::vec3 new_position = event.position + (inverse_force * (new_g - g)) + force_impulse_force;

        vertex_info_buffer_[i].last_event = STREvent(new_position, event.IFR_time + kDt, 
                                                     new_impulse, event.self_time + t0);

        if (tick % (kSkipsNum + 1) == 0)
            trajectory_buffer_[i].addSTREvent(vertex_info_buffer_[i].last_event);
    }
    ++tick;
}

void STREngineCPU::computeSeenSTREvents(const STREvent& observer) {
    for (int i = 0; i < kMaxSTREventCnt; ++i)
        seen_events_buffer_[i] = trajectory_buffer_[i].get_seen_event(observer);
}
