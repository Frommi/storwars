#version 430

const float gradient_step = 0.1;

in vec3 fs_pos;

out vec4 FragColor;

uniform float time;

float density(vec3 pos) {
    float sphere = distance(pos, vec3(0.5)) - 0.48;
    return sphere + sin(pos.x * 32.0 + time * 10) / 100.0;
}

vec3 gradient(vec3 p) {
    vec3 density_p = vec3(density(p));
    vec3 delta = vec3(
        density(p + vec3(gradient_step, 0.0, 0.0)),
        density(p + vec3(0.0, gradient_step, 0.0)),
        density(p + vec3(0.0, 0.0, gradient_step))
    );
    delta -= density_p;

    return normalize(delta);
}

void main() {
    vec3 normal = gradient(fs_pos);

    vec3 gLightDirection = normalize(vec3(-1.0, -1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, normal), 0.0, 1.0);

    vec3 ambientColor = vec3(0.2);
    vec3 diffuseColor = vec3(cosTheta) * 0.8;

    FragColor = vec4(vec3(1.0) * (ambientColor + diffuseColor), 1.0);
}
