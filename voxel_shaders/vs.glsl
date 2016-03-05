#version 430

layout(location = 0) in vec4 Position;

out vec3 fs_pos;

uniform mat4 u_W;
uniform mat4 u_VP;

void main() {
    fs_pos = Position.xyz;
    gl_Position = u_VP * u_W * vec4(Position.xyz, 1.0);
}
