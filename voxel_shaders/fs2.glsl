#version 430

flat in ivec2 offset;

out vec4 color;

void main() {
    ivec3 pos = ivec3((gl_FragCoord - vec2(0.5)) / offset, offset.x * x_mult + offset.y);
    
    
}
