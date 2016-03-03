#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

const uint local_size = gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z;
const uint local_size_vertex = (gl_WorkGroupSize.x + 1) * (gl_WorkGroupSize.y + 1) * (gl_WorkGroupSize.z + 1);

const uvec3 vertex_mult = uvec3((gl_WorkGroupSize.y + 1) * (gl_WorkGroupSize.z + 1), gl_WorkGroupSize.z + 1, 1);

const float gradient_step = 2e-3;

uniform uvec3 offset_vertex[local_size_vertex];
uniform uvec3 global_size;

shared float density_cache[local_size_vertex];
shared vec4  hermite_cache[local_size_edge];


layout (std430, binding=0) buffer SSBO { 
    vec3 SSBO_position;
};


float density(vec3 pos) {
    float sphere = distance(pos, vec3(64.2347));
    return sphere;
}

vec3 gradient(vec3 p) {
    vec3 density_p = vec3(density(p));
    vec3 delta = vec3(
        density(p + vec3(gradient_step, 0.0, 0.0)),
        density(p + vec3(0.0, gradient_step, 0.0)),
        density(p + vec3(0.0, 0.0, gradient_step))
    );
    delta -= density_p;

    return -normalize(delta);
}

void addVertex(uvec3 id, vec3 position) {
    SSBO_positions[dot(id, uvec3(global_size.y * global_size.z, global_size.z, 1))] = position;
}

void main() {
    uvec3 pos = gl_WorkGroupID * gl_WorkGroupSize + offset_vertex[gl_LocalInvocationIndex];
    density_cache[gl_LocalInvocationIndex] = density(vec3(pos));

    if (gl_LocalInvocationIndex + local_size < local_size_vertex) {
        pos = gl_WorkGroupID * gl_WorkGroupSize + offset_vertex[gl_LocalInvocationIndex + local_size];
        density_cache[gl_LocalInvocationIndex + local_size] = density(vec3(pos));
    }

    barrier();
/*
    vec4 densities = vec4(
        density_cache[dot(gl_LocalInvocationID + uvec3(1, 0, 0), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(0, 1, 0), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(0, 0, 1), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID                 , vertex_mult)]
    );

    vec3 pos = vec3(gl_LocalInvocationID);
    vec3 signs = densities.xyz * densities.w;
    vec3 diff = densities.xyz - densities.www;
    vec3 alpha = clamp(densities.xyz / diff, 0.0, 1.0);

    vec4 hx = vec4(0.0, 0.0, 0.0, -1.0);
    vec4 hy = vec4(0.0, 0.0, 0.0, -1.0);
    vec4 hz = vec4(0.0, 0.0, 0.0, -1.0);

    if (signs.x <= 0.0) hx = vec4(gradient(pos + vec3(alpha.x, 0.0, 0.0)), alpha.x);
    if (signs.y <= 0.0) hy = vec4(gradient(pos + vec3(0.0, alpha.y, 0.0)), alpha.y);
    if (signs.z <= 0.0) hz = vec4(gradient(pos + vec3(0.0, 0.0, alpha.z)), alpha.z);

    hermite_x[gl_GlobalInvocationID] = hx;
    hermite_y[gl_GlobalInvocationID] = hy;
    hermite_z[gl_GlobalInvocationID] = hz;

    barrier();

    vec4 e1 = hermite_x[];
*/

    vec4 d_z0 = vec4(
        density_cache[dot(gl_LocalInvocationID                 , vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(1, 0, 0), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(0, 1, 0), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(1, 1, 0), vertex_mult)]
    );

    vec4 d_z1 = vec4(
        density_cache[dot(gl_LocalInvocationID + uvec3(0, 0, 1), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(1, 0, 1), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(0, 1, 1), vertex_mult)],
        density_cache[dot(gl_LocalInvocationID + uvec3(1, 1, 1), vertex_mult)]
    );

    vec4 d_y0 = vec4(d_z0.xy, d_z1.xy);
    vec4 d_y1 = vec4(d_z0.zw, d_z1.zw);

    vec4 d_x0 = vec4(d_z0.xz, d_z1.xz);
    vec4 d_x1 = vec4(d_z0.yw, d_z1.yw);

    vec4 alpha_x = clamp(d_x1 / (d_x1 - d_x0), 0.0, 1.0);
    vec4 alpha_y = clamp(d_y1 / (d_y1 - d_y0), 0.0, 1.0);
    vec4 alpha_z = clamp(d_z1 / (d_z1 - d_z0), 0.0, 1.0);

    vec4 signs_x = -clamp(sign(d_x0 * d_x1) - vec4(1.0), -1.0, 0.0);
    vec4 signs_y = -clamp(sign(d_y0 * d_y1) - vec4(1.0), -1.0, 0.0);
    vec4 signs_z = -clamp(sign(d_z0 * d_z1) - vec4(1.0), -1.0, 0.0);

    alpha_x *= signs_x;
    alpha_y *= signs_y;
    alpha_z *= signs_z;

    float cnt = dot(signs_x, vec4(1.0)) + dot(signs_y, vec4(1.0)) + dot(signs_z, vec4(1.0));

    if (cnt > 0.5) {
        vec3 position = vec3(
            dot(alpha_x, vec4(1.0)), 
            dot(alpha_y, vec4(1.0)), 
            dot(alpha_z, vec4(1.0))
        ) / cnt;

        addVertex(gl_GlobalInvocationID, vec3(gl_GlobalInvocationID) + position);
    }

//////////////////////////////////////////////////////////////////////////////

    if (signs_x.x == 1.0) addQuad();

/*    
    vec4 densities = vec4(
        density_cache[gl_LocalInvocationID.x + 1u][gl_LocalInvocationID.y     ][gl_LocalInvocationID.z     ],
        density_cache[gl_LocalInvocationID.x     ][gl_LocalInvocationID.y + 1u][gl_LocalInvocationID.z     ],
        density_cache[gl_LocalInvocationID.x     ][gl_LocalInvocationID.y     ][gl_LocalInvocationID.z + 1u],
        density_cache[gl_LocalInvocationID.x     ][gl_LocalInvocationID.y     ][gl_LocalInvocationID.z     ]
    );

    vec3 pos = vec3(gl_LocalInvocationID);
    vec3 signs = densities.xyz * densities.w;
    vec3 diff = densities.xyz - densities.www;
    vec3 alpha = clamp(densities.xyz / diff, 0.0, 1.0);

    vec4 hx = vec4(0.0, 0.0, 0.0, -1.0);
    vec4 hy = vec4(0.0, 0.0, 0.0, -1.0);
    vec4 hz = vec4(0.0, 0.0, 0.0, -1.0);

    if (signs.x <= 0.0) hx = vec4(gradient(pos + vec3(alpha.x, 0.0, 0.0)), alpha.x);
    if (signs.y <= 0.0) hy = vec4(gradient(pos + vec3(0.0, alpha.y, 0.0)), alpha.y);
    if (signs.z <= 0.0) hz = vec4(gradient(pos + vec3(0.0, 0.0, alpha.z)), alpha.z);

    hermite_x[gl_GlobalInvocationID] = hx;
    hermite_y[gl_GlobalInvocationID] = hy;
    hermite_z[gl_GlobalInvocationID] = hz;

    barrier();

    vec4 e1 = hermite_x[];
*/
}
