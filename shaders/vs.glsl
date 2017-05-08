#version 430

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;

out vec3 tcs_position;
out vec2 tcs_UV;
out vec3 tcs_normal;
out float tcs_lorentz;

uniform mat4 u_W;

uniform vec3 u_ObsPos;
uniform vec3 u_ObsImpulse;
uniform float u_InverseG;
uniform float u_ObsIFRTime;
uniform float u_MapSize;
uniform vec3 u_ObjVelocity;

vec3 getLorentz() {
    vec3 r = -u_ObsPos - tcs_position;
    return r + u_ObsImpulse * (length(r) + dot(r, u_ObsImpulse) * u_InverseG);
}

vec3 rot(float a, vec3 v) {
    return v + a * u_ObjVelocity;
}

void main() {
    u_ObsIFRTime + u_MapSize + u_ObjVelocity.x;
    tcs_position = (u_W * vec4(Position * 0.1, 1.0)).xyz;
//    tcs_position *= 0.1;
        
/*

    float l = -10000.0 - u_MapSize * 0.00001;
    float r = u_ObsIFRTime;
    for (int i = 0; i < 0; ++i) {
        float m = (l + r) * 0.5;
        vec3 p = rot(m, tcs_position);
        if (length(u_ObsPos + p) + m <= u_ObsIFRTime) {
            l = m;
        } else {
            r = m;
        }
    }
    tcs_position = rot(l, tcs_position);

*/

    tcs_lorentz = length(getLorentz());
    tcs_normal = Normal;
    //tcs_normal = rot(l, tcs_normal);
    tcs_UV = UV;
}
