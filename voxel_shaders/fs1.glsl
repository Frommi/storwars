#version 430

flat in ivec2 offset;

out vec4 color;

uniform int x_mult;
uniform float eps;

void main() {
    ivec3 pos = ivec3((gl_FragCoord - vec2(0.5)) / offset, offset.x * x_mult + offset.y);

    vec4 density = vec4(
        f(pos + vec3(1.0, 0.0, 0.0)), 
        f(pos + vec3(0.0, 1.0, 0.0)), 
        f(pos + vec3(0.0, 0.0, 1.0)), 
        f(pos)
    );

    vec3 signs = clamp(sign(density.xyz * (-density.w)), 0.0, 1.0);
    float w = dot(signs, vec3(4.0, 2.0, 1.0));

    vec3 diff = density.xyz - density.www;
    color = vec4(clamp(density.xyz / diff, 0.0, 1.0), w);
}
