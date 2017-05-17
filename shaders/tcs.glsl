#version 430

layout(vertices = 3) out;

in vec3 tcs_position[];
in vec2 tcs_UV[];
in vec3 tcs_normal[];
in float tcs_lorentz[];

out vec3 tes_position[];
out vec2 tes_UV[];
out vec3 tes_normal[];

uniform vec3 u_ObsPos;

float getTessLevel(float d, float l) {
    //return 1;
    return min(8, (l * 64.0) / (d + 0.2));
}

void main() {
    tes_position[gl_InvocationID] = tcs_position[gl_InvocationID];
    tes_UV[gl_InvocationID]       = tcs_UV[gl_InvocationID];
    tes_normal[gl_InvocationID]   = tcs_normal[gl_InvocationID];

    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = getTessLevel(tcs_lorentz[1] + tcs_lorentz[2], distance(tcs_position[1], tcs_position[2]));
        gl_TessLevelOuter[1] = getTessLevel(tcs_lorentz[2] + tcs_lorentz[0], distance(tcs_position[2], tcs_position[0]));
        gl_TessLevelOuter[2] = getTessLevel(tcs_lorentz[0] + tcs_lorentz[1], distance(tcs_position[0], tcs_position[1]));
        gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    }
}
