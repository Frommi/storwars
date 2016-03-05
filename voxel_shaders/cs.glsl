#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

const float s = 32.0;
const uint local_size = gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z;
const uint local_size_vertex = (gl_WorkGroupSize.x + 1) * (gl_WorkGroupSize.y + 1) * (gl_WorkGroupSize.z + 1);
const vec3 density_mult = vec3((float(gl_WorkGroupSize.y) + 1.0) * (float(gl_WorkGroupSize.z) + 1.0), float(gl_WorkGroupSize.z) + 1.0, 1.0);


layout (std430, binding = 0) buffer SSBO_pos { 
    vec4 SSBO_position[];
};

layout (std430, binding = 1) buffer SSBO_ind {
    uvec2 SSBO_index[];
};

layout(binding = 2) uniform atomic_uint tri_cnt;

uniform uvec3 size;
shared float density_cache[local_size_vertex];

uvec3 offset(uint id) {
    uvec3 res = (id.xxx / uvec3(9*9, 9*9, 9)) * uvec3(1, 9, 9);
    res.z = id - res.z;
    res.y = id / 9 - res.y;
    return res;
}

float density(vec3 pos) {
    float sphere = distance(pos, vec3(s)) - (s - 1.238746);
    return sphere;
}

void addVertex(uvec3 id, vec3 position) {
    SSBO_position[id.x * size.y * size.z + id.y * size.z + id.z] = vec4(position / 10.0, 0.0);
}

void addQuad(ivec3 a, ivec3 b, ivec3 c, ivec3 d) {
    uvec2 global_mult = uvec2(size.y * size.z, size.z);

    uint i = atomicCounterIncrement(tri_cnt);
    uvec4 indices = uvec4(a.x, b.x, c.x, d.x) * global_mult.xxxx + 
                    uvec4(a.y, b.y, c.y, d.y) * global_mult.yyyy + 
                    uvec4(a.z, b.z, c.z, d.z);

    SSBO_index[i * 3 + 0] = indices.xy;
    SSBO_index[i * 3 + 1] = indices.zx;
    SSBO_index[i * 3 + 2] = indices.wz;
}

void main() {
    vec3 pos = vec3(gl_WorkGroupID * gl_WorkGroupSize + offset(gl_LocalInvocationIndex));
    density_cache[gl_LocalInvocationIndex] = density(pos);

    if (gl_LocalInvocationIndex + local_size < local_size_vertex) {
        pos = vec3(gl_WorkGroupID * gl_WorkGroupSize + offset(gl_LocalInvocationIndex + local_size));
        density_cache[gl_LocalInvocationIndex + local_size] = density(pos);
    }

    memoryBarrierShared();
    barrier();

    pos = vec3(gl_LocalInvocationID);

    vec4 d_z0 = vec4(
        density_cache[uint(dot(pos                      , density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 0.0, 0.0), density_mult))],
        density_cache[uint(dot(pos + vec3(0.0, 1.0, 0.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 1.0, 0.0), density_mult))]
    );

    vec4 d_z1 = vec4(
        density_cache[uint(dot(pos + vec3(0.0, 0.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 0.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(0.0, 1.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 1.0, 1.0), density_mult))]
    );

    vec4 d_y0 = vec4(d_z0.xy, d_z1.xy);
    vec4 d_y1 = vec4(d_z0.zw, d_z1.zw);

    vec4 d_x0 = vec4(d_z0.xz, d_z1.xz);
    vec4 d_x1 = vec4(d_z0.yw, d_z1.yw);

    vec4 alpha_x = clamp(d_x0 / (d_x0 - d_x1), 0.0, 1.0);
    vec4 alpha_y = clamp(d_y0 / (d_y0 - d_y1), 0.0, 1.0);
    vec4 alpha_z = clamp(d_z0 / (d_z0 - d_z1), 0.0, 1.0);

    vec4 signs_x = -clamp(sign(d_x0 * d_x1) - vec4(1.0), -1.0, 0.0);
    vec4 signs_y = -clamp(sign(d_y0 * d_y1) - vec4(1.0), -1.0, 0.0);
    vec4 signs_z = -clamp(sign(d_z0 * d_z1) - vec4(1.0), -1.0, 0.0);

    alpha_x *= signs_x;
    alpha_y *= signs_y;
    alpha_z *= signs_z;

    float cnt = dot(signs_x, vec4(1.0)) + dot(signs_y, vec4(1.0)) + dot(signs_z, vec4(1.0));

    if (cnt > 0.5) {
        vec3 position = vec3(
            dot(alpha_x, vec4(1.0)) + dot(vec4(signs_y.yw, signs_z.yw), vec4(1.0)),
            dot(alpha_y, vec4(1.0)) + dot(vec4(signs_x.yw, signs_z.zw), vec4(1.0)),
            dot(alpha_z, vec4(1.0)) + dot(vec4(signs_x.zw, signs_y.zw), vec4(1.0))
        ) / cnt;

        addVertex(gl_GlobalInvocationID, vec3(gl_GlobalInvocationID) + position);
    }


    ivec3 ipos = ivec3(gl_GlobalInvocationID);

    if (signs_x.x == 1.0 && ipos.y * ipos.z != 0) addQuad(
        ipos, 
        ipos + ivec3(0, -1, 0),
        ipos + ivec3(0, -1, -1),
        ipos + ivec3(0, 0, -1)
    );

    if (signs_y.x == 1.0 && ipos.x * ipos.z != 0) addQuad(
        ipos, 
        ipos + ivec3(-1, 0, 0),
        ipos + ivec3(-1, 0, -1),
        ipos + ivec3(0, 0, -1)
    );

    if (signs_z.x == 1.0 && ipos.x * ipos.y != 0) addQuad(
        ipos, 
        ipos + ivec3(-1, 0, 0),
        ipos + ivec3(-1, -1, 0),
        ipos + ivec3(0, -1, 0)
    );  
}
