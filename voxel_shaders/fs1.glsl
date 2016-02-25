#version 430

in vec2 offset;
in float z;

out vec4 color;

uniform float gradient_step;

vec3 gradient(vec3 p) {
    vec3 fp = vec3(f(p));
    vec3 delta = vec3(
        f(p + vec3(gradient_step, 0.0, 0.0)),
        f(p + vec3(0.0, gradient_step, 0.0)),
        f(p + vec3(0.0, 0.0, gradient_step))
    );
    delta -= fp;

    return -normalize(delta);
}

void main() {
    vec3 pos = vec3(gl_FragCoord - offset, z);

    vec4 density = vec4(
        f(pos + vec3(1.0, 0.0, 0.0)), 
        f(pos + vec3(0.0, 1.0, 0.0)), 
        f(pos + vec3(0.0, 0.0, 1.0)), 
        f(pos)
    );

    vec3 signs = density.xyz * density.w;

    vec3 diff = density.xyz - density.www;
    vec3 alpha = clamp(density.xyz / diff, 0.0, 1.0);

    color_x = vec4(0.0, 0.0, 0.0, -1.0);
    color_y = vec4(0.0, 0.0, 0.0, -1.0);
    color_z = vec4(0.0, 0.0, 0.0, -1.0);

    if (signs.x <= 0.0) {
        vec3 normal_x = gradient(pos + vec3(alpha.x, 0.0, 0.0));
        vec4 color_x = vec4(normal_x, alpha.x);
    }

    if (signs.y <= 0.0) {
        vec3 normal_y = gradient(pos + vec3(0.0, alpha.y, 0.0));
        vec4 color_y = vec4(normal_y, alpha.y);
    }

    if (signs.z <= 0.0) {
        vec3 normal_z = gradient(pos + vec3(0.0, 0.0, alpha.z));
        vec4 color_z = vec4(normal_z, alpha.z);
    }
}
