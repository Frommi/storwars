#version 430

in vec2 fs_UV;
in vec3 fs_normal;
in vec3 fs_bari;
in vec3 fs_tri;
in vec3 fs_position;

uniform sampler2D diffuseTexture;
uniform vec3 u_ObsImpulse;

out vec4 color;

float amplify(float d, float scale, float offset) {
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2 * d * d);
    return d;
}

float getWave(float l) {
    float inv_g = inversesqrt(1 + dot(u_ObsImpulse, u_ObsImpulse));
    float cosT = dot(u_ObsImpulse, fs_position) / (length(u_ObsImpulse) * length(fs_position));
    cosT = clamp(-1.0, 1.0, cosT);
    return l * (inv_g / (1.0 + length(u_ObsImpulse) * cosT / inv_g));
}

vec3 rgbByWave(float l) {
    vec3 res;
    float t;
         if ((l>=400.0)&&(l<410.0)) { t=(l-400.0)/(410.0-400.0); res.x=    +(0.33*t)-(0.20*t*t); }
    else if ((l>=410.0)&&(l<475.0)) { t=(l-410.0)/(475.0-410.0); res.x=0.14         -(0.13*t*t); }
    else if ((l>=545.0)&&(l<595.0)) { t=(l-545.0)/(595.0-545.0); res.x=    +(1.98*t)-(     t*t); }
    else if ((l>=595.0)&&(l<650.0)) { t=(l-595.0)/(650.0-595.0); res.x=0.98+(0.06*t)-(0.40*t*t); }
    else if ((l>=650.0)&&(l<700.0)) { t=(l-650.0)/(700.0-650.0); res.x=0.65-(0.84*t)+(0.20*t*t); }
         if ((l>=415.0)&&(l<475.0)) { t=(l-415.0)/(475.0-415.0); res.y=             +(0.80*t*t); }
    else if ((l>=475.0)&&(l<590.0)) { t=(l-475.0)/(590.0-475.0); res.y=0.8 +(0.76*t)-(0.80*t*t); }
    else if ((l>=585.0)&&(l<639.0)) { t=(l-585.0)/(639.0-585.0); res.y=0.84-(0.84*t)           ; }
         if ((l>=400.0)&&(l<475.0)) { t=(l-400.0)/(475.0-400.0); res.z=    +(2.20*t)-(1.50*t*t); }
    else if ((l>=475.0)&&(l<560.0)) { t=(l-475.0)/(560.0-475.0); res.z=0.7 -(     t)+(0.30*t*t); }

    return res;
}

void main() {
    vec3 gLightDirection = normalize(vec3(-1.0, 1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, fs_normal), 0, 1);

    vec3 ambientColor = vec3(0.2);
    vec3 diffuseColor = vec3(cosTheta) * 0.8;

    color = vec4(texture2D(diffuseTexture, fs_UV.xy).xyz * (ambientColor + diffuseColor), 1.0);
    //color = vec4(vec3(1.0) * (ambientColor + diffuseColor), 1.0);
    //color = vec4(vec3(cosTheta) * (ambientColor + diffuseColor), 1.0);

    float d1 = min(min(fs_tri.x, fs_tri.y), fs_tri.z);
    float d2 = min(min(fs_bari.x, fs_bari.y), fs_bari.z);

//    color = vec4(rgbByWave(getWave(650.0)) * color.x + 
//               rgbByWave(getWave(510.0)) * color.y +
//               rgbByWave(getWave(475.0)) * color.z, 1.0);
    
//    color = vec4(rgbByWave(getWave(575.0)), 1.0);

    color *= amplify(d2, 60, -0.5) * amplify(d1, 40, -0.5);
}
