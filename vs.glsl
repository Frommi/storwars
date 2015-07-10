#version 430

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;

out vec3 normal;

uniform mat4 u_WVP;

void main() {
    gl_Position = u_WVP * vec4(Position, 1.0);
    normal = Normal;
}
