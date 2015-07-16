#include "game_state.h"

void GameFrame::allocateNewBody(STRBody& body) {
    int i = 0;
    while (i != static_cast<int>(body.frameIndicies.size())) {
        if (free_indicies_.size() == 0) {
            fprintf(stderr, "Out of free events!\n");
            exit(EXIT_FAILURE);
        }

        body.frameIndicies[i] = free_indicies_.back();
        free_indicies_.pop_back();
        ++i;
    }
}

void GameFrame::deleteOldBody(STRBody& body) {
    for (int i = 0; i < static_cast<int>(body.frameIndicies.size()); ++i) {
        free_indicies_.push_back(body.frameIndicies[i]);
    }
}
