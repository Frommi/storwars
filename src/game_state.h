#pragma once

#include "static_mesh.h"
#include "str_event.h"
#include "str_engine.h"

class GameState {
public:
    GameState() {}

    ~GameState() = default;

    void tick(float dt);

private:
    std::vector<STRBody> bodies_;
};
