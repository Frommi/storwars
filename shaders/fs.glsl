#version 430

in vec3 normal;
in vec2 uv;

uniform sampler2D diffuseTexture;

out vec4 color;

void main() {
    vec3 gLightDirection = normalize(vec3(1.0, 1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, normal), 0, 1);

    vec3 ambientColor = vec3(0.2, 0.2, 0.2);
    vec3 diffuseColor = vec3(cosTheta, cosTheta, cosTheta) * 0.5;

    color = vec4(texture2D(diffuseTexture, uv.xy) * (ambientColor + diffuseColor), 1.0);
}
