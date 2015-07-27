#pragma once

#include "static_mesh.h"
#include "str_event.h"
#include "str_engine.h"
#include "str_engine_cpu.h"

class GameState {
public:
    GameState(float dt) : 
        dt(dt)
    {}

    ~GameState() = default;

private:
    float dt;
    std::vector<STRBody> bodies_;
};
