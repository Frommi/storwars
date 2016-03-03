#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (std430, binding = 0) buffer SSBO_pos { 
    vec4 SSBO_position[];
};

layout (std430, binding = 1) buffer SSBO_ind {
    uint SSBO_index[];
};

layout(binding = 2) uniform atomic_uint tri_cnt;

uniform uvec3 size;

float density(vec3 pos) {
    float sphere = distance(pos, vec3(64.0)) - 62.5133454;
    return sphere;
}

void addVertex(uvec3 id, vec3 position) {
    SSBO_position[id.x * size.y * size.z + id.y * size.z + id.z] = vec4(position / 10.0, 0.0);
    //SSBO_position[uint(dot(id, uvec3(size.y * size.z, size.z, 1)))] = position;
}

void addQuad(ivec3 a, ivec3 b, ivec3 c, ivec3 d) {
    if (a.x < 0 || a.y < 0 || a.z < 0) return;
    if (b.x < 0 || b.y < 0 || b.z < 0) return;
    if (c.x < 0 || c.y < 0 || c.z < 0) return;
    if (d.x < 0 || d.y < 0 || d.z < 0) return;

    uvec3 global_mult = uvec3(size.y * size.z, size.z, 1);

    uint i = atomicCounterIncrement(tri_cnt);
    SSBO_index[i * 6 + 0] = a.x * global_mult.x + a.y * global_mult.y + a.z + 1;
    SSBO_index[i * 6 + 1] = b.x * global_mult.x + b.y * global_mult.y + b.z + 1;
    SSBO_index[i * 6 + 2] = c.x * global_mult.x + c.y * global_mult.y + c.z + 1;
    SSBO_index[i * 6 + 3] = a.x * global_mult.x + a.y * global_mult.y + a.z + 1;
    SSBO_index[i * 6 + 4] = d.x * global_mult.x + d.y * global_mult.y + d.z + 1;
    SSBO_index[i * 6 + 5] = c.x * global_mult.x + c.y * global_mult.y + c.z + 1;

    /*
    SSBO_index[atomicCounterIncrement(tri_cnt)] = uvec4(
        a.x * global_mult.x + a.y * global_mult.y + a.z,
        b.x * global_mult.x + b.y * global_mult.y + b.z,
        c.x * global_mult.x + c.y * global_mult.y + c.z,
        d.x * global_mult.x + d.y * global_mult.y + d.z
        //dot(uvec3(a), global_mult),
        //dot(uvec3(b), global_mult),
        //dot(uvec3(c), global_mult)
    );*/
/*
    SSBO_index[atomicCounterIncrement(tri_cnt)] = uvec3(2, 3, 4
        b.x * global_mult.x + b.y * global_mult.y + b.z,
        c.x * global_mult.x + c.y * global_mult.y + c.z,
        d.x * global_mult.x + d.y * global_mult.y + d.z
    //    dot(uvec3(b), global_mult),
    //    dot(uvec3(c), global_mult),
    //    dot(uvec3(d), global_mult)
    );*/
}

void main() {
    addVertex(gl_GlobalInvocationID, vec3(gl_GlobalInvocationID) + vec3(0.5));

    vec3 pos = vec3(gl_GlobalInvocationID);
    vec4 d = vec4(
        density(pos + vec3(1.0, 0.0, 0.0)),
        density(pos + vec3(0.0, 1.0, 0.0)),
        density(pos + vec3(0.0, 0.0, 1.0)),
        density(pos)
    );

    vec3 signs = d.xyz * d.www;

    if (signs.x <= 0.0) addQuad(
        ivec3(gl_GlobalInvocationID), 
        ivec3(gl_GlobalInvocationID) + ivec3(0, -1, 0),
        ivec3(gl_GlobalInvocationID) + ivec3(0, -1, -1),
        ivec3(gl_GlobalInvocationID) + ivec3(0, 0, -1)
    );

    if (signs.y <= 0.0) addQuad(
        ivec3(gl_GlobalInvocationID), 
        ivec3(gl_GlobalInvocationID) + ivec3(-1, 0, 0),
        ivec3(gl_GlobalInvocationID) + ivec3(-1, 0, -1),
        ivec3(gl_GlobalInvocationID) + ivec3(0, 0, -1)
    );

    if (signs.z <= 0.0) addQuad(
        ivec3(gl_GlobalInvocationID), 
        ivec3(gl_GlobalInvocationID) + ivec3(-1, 0, 0),
        ivec3(gl_GlobalInvocationID) + ivec3(-1, -1, 0),
        ivec3(gl_GlobalInvocationID) + ivec3(0, -1, 0)
    );  
}
