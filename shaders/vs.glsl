#version 430

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;

out vec3 tcs_position;
out vec2 tcs_UV;
out vec3 tcs_normal;
out vec3 tcs_lorentz;

uniform mat4 u_W;

uniform vec3 u_ObsPos;
uniform vec3 u_ObsImpulse;
uniform float u_InverseG;

vec3 getLorentz() {
	vec3 r = -u_ObsPos - tcs_position;
	return r + u_ObsImpulse * (length(r) + dot(r, u_ObsImpulse) * u_InverseG);
}

void main() {
	tcs_position = (u_W * vec4(Position, 1.0)).xyz;
	tcs_lorentz = getLorentz();
    tcs_normal = Normal;
    tcs_UV = UV;
}
