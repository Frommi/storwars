#pragma once

#include "static_mesh.h"

struct STRBody {
    STRBody(const StaticMesh& static_mesh) {
        mesh = &static_mesh;

        int vertices_cnt = 0;
        for (int i = 0; i < static_cast<int>(static_mesh.homo_meshes_.size()); ++i)
            vertices_cnt += static_cast<int>(static_mesh.homo_meshes_[i].vertices.size());
        frameIndicies.resize(vertices_cnt);
    }
    
    std::vector<int> frameIndicies;
    const StaticMesh* mesh;
};


class GameFrame {
public:
    GameFrame(int size) {
        events_.resize(size);
    }
    ~GameFrame() = default;

    void allocateNewBody(STRBody& body);
    void deleteOldBody(STRBody& body);

private:
    std::vector<STREvent> events_;

    std::vector<int> free_indicies_;
};


class GameState {
public:
    GameState(float dt) : 
        dt(dt), 
        last_frame_(50000),
        frame_num_(0)
    {}

    ~GameState() = default;

    inline float get_last_iso_time() const { return frame_num_ * dt; }

private:
    float dt;

    GameFrame last_frame_;
    int frame_num_;

    std::vector<STRBody> bodies_;
};
