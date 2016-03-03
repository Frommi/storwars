#version 430

layout(location = 0) in vec4 Position;

uniform mat4 u_W;
uniform mat4 u_VP;

void main() {
    gl_Position = u_VP * u_W * vec4(Position.xyz, 1.0);
}
