#version 430

layout(triangles, equal_spacing, ccw) in;

in vec3 tes_position[];
in vec2 tes_UV[];
in vec3 tes_normal[];

out vec3 gs_position;
out vec2 gs_UV;
out vec3 gs_normal;
out vec3 gs_bari;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {                                                                                               
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main() {
    gs_position = interpolate3D(tes_position[0], tes_position[1], tes_position[2]);
    gs_UV = interpolate2D(tes_UV[0], tes_UV[1], tes_UV[2]);
    gs_normal = interpolate3D(tes_normal[0], tes_normal[1], tes_normal[2]);
    gs_normal = normalize(gs_normal);
    gs_bari = gl_TessCoord;
}
