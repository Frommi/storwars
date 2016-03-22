#version 430

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v) { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;

  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  float n_ = 0.142857142857;
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

vec4 dsnoise(vec3 v) { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;

  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  float n_ = 0.142857142857;
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  vec4 m2 = m * m;
  vec4 m4 = m2 * m2;
  vec4 pdotx = vec4(dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3));

  vec4 temp = m2 * m * pdotx;
  vec3 gradient = -8.0 * (temp.x * x0 + temp.y * x1 + temp.z * x2 + temp.w * x3);
  gradient += m4.x * p0 + m4.y * p1 + m4.z * p2 + m4.w * p3;
  return 42.0 * vec4(dot(m4, pdotx), gradient);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float gradient_step = 0.00001;

in vec3 fs_pos;

out vec4 FragColor;

uniform float time;

float density(vec3 pos) {
    float sphere = distance(pos, vec3(0.5)) - 0.45;
    sphere *= 0.5;

    sphere += snoise(pos * 510.52347) * 0.0003125 * 0.1;
    sphere += snoise(pos * 257.98141) * 0.000625 * 0.1;
    sphere += snoise(pos * 128.0141) * 0.00125 * 0.1;

    //sphere += snoise(pos * 64.7347) * 0.0025;
    //sphere += snoise(pos * 32.4124) * 0.005;
    sphere += cos(pos.x * 100 + snoise(pos * 3.81419) * 8) * 0.01;

    //sphere += snoise(pos * 8.12347) * 0.02;
    //sphere += snoise(pos * 4.98141) * 0.04;
    return sphere;
}

vec3 gradient_analytical(vec3 pos) {
    vec3 res = normalize(pos - vec3(0.5));
    res *= 0.5;

    //res += dsnoise(pos * 2037.7991).yzw * 0.016012 * 0.75;
    res += dsnoise(pos * 1029.3047).yzw * 0.016002 * 1;
    res += dsnoise(pos * 510.52347).yzw * 0.015953 * 1.5;
    res += dsnoise(pos * 257.98141).yzw * 0.016123 * 1.5 * 1.5;
    //res += dsnoise(pos * 128.0141).yzw * 0.0160017 * 32;

    vec4 dsn = dsnoise(pos * 3.81419) * 8;
    dsn.yzw *= 3.81419;
    float sinpos = -sin(pos.x * 100 + dsn.x) * 0.01;
    res += sinpos * (dsn.yzw + vec3(100.0, 0.0, 0.0));

    return normalize(res);
}

/*
float density(vec3 pos) {
    float sphere = distance(pos, vec3(0.5)) - 0.48;
    return sphere + sin(pos.x * 32.0 + time * 5) / 100.0 + sin(pos.y * 128.0 + time * 10) / 300.0 + sin(pos.z * 512.0 + time * 20) / 500.0;
}
*/

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
    // vec3 normal = gradient(fs_pos);
    vec3 normal = gradient_analytical(fs_pos);

    vec3 gLightDirection = normalize(vec3(-1.0, -1.0, 0.7));

    float cosTheta = clamp(dot(gLightDirection, normal), 0.0, 1.0);

    float occlusion = clamp((clamp(distance(fs_pos, vec3(0.5)), 0.3, 1.0) - 0.4) * 10.0, 0.0, 1.0);
    vec3 ambientColor = vec3(0.2);
    vec3 diffuseColor = vec3(cosTheta) * 0.8;

    FragColor = vec4(vec3(0.6 + occlusion * 0.4) * (ambientColor + diffuseColor), 1.0);
}
