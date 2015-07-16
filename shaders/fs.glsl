#version 430

in vec3 normal;
out vec4 color;

void main() {
    vec3 gLightDirection = normalize(vec3(1.0, 1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, normal), 0, 1);

    color = vec4(0.2 + cosTheta * 0.5, 0.0, 0.0, 1.0);
}
