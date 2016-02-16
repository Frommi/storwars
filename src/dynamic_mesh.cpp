#include "dynamic_mesh.h"

void DynamicVertex::addBone(uint id, float weight) {
    int i = 0;
    while (bone_weights[i] > EPS) {
        ++i;
        if (i == 4) {
            fprintf(stderr, "Too many bones mapping for one vertex");
            exit(1);
        }
    }

    boneIDs[i] = id;
    bone_weights[i] = weight;
}

