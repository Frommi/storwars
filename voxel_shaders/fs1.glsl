#version 430

in vec2 offset;

out vec4 color;

uniform vec2 offset_z_k;
uniform float gradient_step;
uniform float gradient_step_inv;

vec3 gradient(vec3 p) {
    float fp = f(p);
    vec3 delta = vec3(
        f(p + vec3(gradient_step, 0.0, 0.0)),
        f(p + vec3(0.0, gradient_step, 0.0)),
        f(p + vec3(0.0, 0.0, gradient_step))
    );
    delta -= fp.xxx;

    return normalize(delta);
}

void main() {
    vec3 pos = vec3(gl_FragCoord - offset, dot(offset, offset_z_k));

    vec4 density = vec4(
        f(pos + vec3(1.0, 0.0, 0.0)), 
        f(pos + vec3(0.0, 1.0, 0.0)), 
        f(pos + vec3(0.0, 0.0, 1.0)), 
        f(pos)
    );

    vec3 signs = clamp(sign(density.xyz * (-density.w)), 0.0, 1.0);
    float w = dot(signs, vec3(4.0, 2.0, 1.0));

    vec3 diff = density.xyz - density.www;
    vec3 alpha = clamp(density.xyz / diff, 0.0, 1.0);


    if (signs.x == 1.0) {
        vec3 normal_x = gradient(pos + vec3(alpha.x, 0.0, 0.0));
        vec4 color_x = vec4(normal_x, alpha.x);
    } else {
        color_x = vec4(0.0, 0.0, 0.0, -1.0);
    }

    if (signs.y == 1.0) {
        vec3 normal_y = gradient(pos + vec3(0.0, alpha.y, 0.0));
        vec4 color_y = vec4(normal_y, alpha.y);
    } else {
        color_y = vec4(0.0, 0.0, 0.0, -1.0);
    }

    if (signs.z == 1.0) {
        vec3 normal_z = gradient(pos + vec3(0.0, 0.0, alpha.z));
        vec4 color_z = vec4(normal_z, alpha.z);
    } else {
        color_z = vec4(0.0, 0.0, 0.0, -1.0);
    }
}
