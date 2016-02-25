#version 430

layout(location = 0) in vec4 Position;

out vec2 offset;
out float z;

uniform vec2 offset_mult;

void main() {
    offset = Position.zw;
    z = dot(offset, offset_mult);
    vec2 xy = Position.xy + offset;
    gl_Position = vec4(, 1.0);    
}
