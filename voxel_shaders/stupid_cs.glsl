#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (std430, binding = 0) buffer SSBO_pos { 
    vec3 SSBO_position[];
};

layout (std430, binding = 1) buffer SSBO_ind {
    uvec3 SSBO_index[];
};

layout(binding = 2) uniform atomic_uint tri_cnt;

uniform uvec3 size;

float density(vec3 pos) {
    float sphere = distance(pos, vec3(64.2347));
    return sphere;
}

void addVertex(uvec3 id, vec3 position) {
    SSBO_position[int(dot(id, uvec3(size.y * size.z, size.z, 1)))] = position;
}

void addQuad(ivec3 a, ivec3 b, ivec3 c, ivec3 d) {
    if (a.x < 0 || a.y < 0 || a.z < 0) return;
    if (b.x < 0 || b.y < 0 || b.z < 0) return;
    if (c.x < 0 || c.y < 0 || c.z < 0) return;
    if (d.x < 0 || d.y < 0 || d.z < 0) return;

    uvec3 global_mult = uvec3(size.y * size.z, size.z, 1);

    SSBO_index[atomicCounterIncrement(tri_cnt)] = uvec3(
        dot(uvec3(a), global_mult),
        dot(uvec3(b), global_mult),
        dot(uvec3(c), global_mult)
    );

    SSBO_index[atomicCounterIncrement(tri_cnt)] = uvec3(
        dot(uvec3(b), global_mult),
        dot(uvec3(c), global_mult),
        dot(uvec3(d), global_mult)
    );
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