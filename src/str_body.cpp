#include "str_body.h"

void STRVertex::initVertex(const STREvent& event, int mesh_homo_index, int mesh_vertex_index) {
    trajectory_.initTrajectory(event);
    mesh_homo_index_ = mesh_homo_index;
    mesh_vertex_index_ = mesh_vertex_index;
}

void STRVertex::setForce(const glm::vec3 force) {
    force_ = force;
}

void STRVertex::addForce(const glm::vec3 force) {
    force_ += force;
}

void STRVertex::addString(Vertex* end, float tension) {
    STRString string;
    string.end = end;
    string.direction = body_mesh_->homo_meshes_[mesh_homo_index_].vertices[mesh_vertex_index_] - 
        body_mesh_->homo_meshes_[end->mesh_homo_index_].vertices[end->mesh_vertex_index_];
    string.tension = tension;

    strings_.push_back(string);
}


STRBody::STRBody(const StaticMesh* body_mesh):
    body_mesh_(body_mesh)
{}

void STRBody::initBody(const STREvent& eye_event, const glm::quat& sight_direction) {
    vertices_.resize(body_mesh_->homo_meshes_.size());

    for (int i = 0; i < static_cast<int>(body_mesh_->homo_meshes_.size()); ++i)
        for (int j = 0; j < static_cast<int>(body_mesh_->homo_meshes_[i].vertices.size()); ++j) {
            STREvent vertex_event = eye_event;
            vertex_event.position += glm::vec3(glm::mat4_cast(sight_direction) * 
                                              glm::vec4(static_mesh_->homo_meshes_[i].vertices[j], 1.0f));

            vertices_[i].push_back(STRVertex());
            vertices_[i].back().initVertex(vertex_event, i, j);
        }

    prepareStrings();
}

void STRBody::prepareStrings() {
    for (int i = 0; i < static_cast<int>(vertices_.size()); ++i) {
        for (int j = 0; j < static_cast<int>(vertices_[i].size()); ++j) {
            const glm::vec3 triangle = body_mesh_->homo_meshes_[i].indices[j];
            
            for (int p = 0; p < 3; ++p)
                for (int q = 0; q < 3; ++q)
                    if (p != q)
                        vertices_[i][triangle[p]].addString(&vertices_[i][triangle[q]], 0.1f);
        }
    }
}

void STRBody::computeForces() {
    for (int i = 0; i < static_cast<int>(vertices_.size()); ++i)
        for (int j = 0; j < static_cast<int>(vertices_[i].size()); ++j)
            vertices_[i][j].setForce(glm::vec3(0.0f));

    addStringForces();
    addEyeForces();
}

void STRBody::addStringForces() {
    for (int i = 0; i < static_cast<int>(vertices_.size()); ++i) {
        for (int j = 0; j < static_cast<int>(vertices_[i].size()); ++j) {
            STREvent observer = vertices_[i][j].trajectory_.getLastSTREvent();
            for (int k = 0; k < static_cast<int>(vertices_[i][j].strings_.size()); ++k) {
                STREvent seen_string_event vertices_[i][j].strings_[k].end_vertex->trajectory_.getSeenSTREvent(observer);
                (seen_string_event.position - observer.position);
                direction * 
                vertices_[i][j].addForce(...........................);
            }
        }
    }
}

void STRBody::addEyeForces() {
    ...
}
