#version 430

layout(location = 0) in vec3 Position;

uniform mat4 u_W;
uniform mat4 u_VP;

void main() {
    gl_Position = u_VP * u_W * vec4(Position, 1.0);
}
