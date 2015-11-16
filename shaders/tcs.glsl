#version 430

layout (vertices = 3) out;

in vec3 tcs_position[];
in vec2 tcs_UV[];
in vec3 tcs_normal[];
in vec3 tcs_lorentz[];

out vec3 tes_position[];
out vec2 tes_UV[];
out vec3 tes_normal[];

uniform vec3 u_ObsPos;

int getTessLevel(float d0, float d1) {
	float d = (d0 + d1) * 0.5;
	if (d <= 0.5) {
		return 4;
	}
	if (d <= 1.0) {
		return 3;
	}
	return 1;
}

void main() {
    tes_position[gl_InvocationID] = tcs_position[gl_InvocationID];
    tes_UV[gl_InvocationID]       = tcs_UV[gl_InvocationID];
    tes_normal[gl_InvocationID]   = tcs_normal[gl_InvocationID];

    vec3 u_ObsPos1 = vec3(0.0, 0.0, 0.0);
    float d0 = distance(-u_ObsPos1, tcs_lorentz[0]);
    float d1 = distance(-u_ObsPos1, tcs_lorentz[1]);
    float d2 = distance(-u_ObsPos1, tcs_lorentz[2]);

    gl_TessLevelOuter[0] = getTessLevel(d1, d2);
    gl_TessLevelOuter[1] = getTessLevel(d2, d0);
    gl_TessLevelOuter[2] = getTessLevel(d0, d1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}
