#version 430

in vec2 fs_UV;
in vec3 fs_normal;

uniform sampler2D diffuseTexture;

out vec4 color;

void main() {
    vec3 gLightDirection = normalize(vec3(1.0, 1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, fs_normal), 0, 1);

    vec3 ambientColor = vec3(0.2, 0.2, 0.2);
    vec3 diffuseColor = vec3(cosTheta, cosTheta, cosTheta) * 0.8;

    color = vec4(texture2D(diffuseTexture, fs_UV.xy).xyz * (ambientColor + diffuseColor), 1.0);
}
