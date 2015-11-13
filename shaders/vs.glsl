#version 430

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;

out vec3 normal;
out vec2 uv;

uniform mat4 u_W;
uniform mat4 u_VP;
uniform vec3 u_ObsPos;
uniform vec3 u_ObsImpulse;

vec3 getLorentz() {
	vec3 r = -u_ObsPos - gl_Position.xyz;
	float obs_impulse_dot = dot(u_ObsImpulse, u_ObsImpulse);
	vec3 r_parallel_new = u_ObsImpulse * dot(r, u_ObsImpulse) / (1 + sqrt(1 + obs_impulse_dot));
	return r + r_parallel_new + length(r) * u_ObsImpulse;
}

void main() {
	gl_Position = u_W * vec4(Position, 1.0);
    gl_Position = u_VP * vec4(getLorentz(), 1.0);
    normal = Normal;
    uv = UV;
}
