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


layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

const uint local_size = gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z;
const uint local_size_vertex = (gl_WorkGroupSize.x + 1) * (gl_WorkGroupSize.y + 1) * (gl_WorkGroupSize.z + 1);
const vec3 density_mult = vec3((float(gl_WorkGroupSize.y) + 1.0) * (float(gl_WorkGroupSize.z) + 1.0), float(gl_WorkGroupSize.z) + 1.0, 1.0);


layout (std430, binding = 0) buffer SSBO_pos { 
    vec4 SSBO_position[];
};

layout (std430, binding = 1) buffer SSBO_ind {
    uvec2 SSBO_index[];
};

layout(binding = 2) uniform atomic_uint tri_cnt;

uniform uvec3 size;
uniform float time;
shared float density_cache[local_size_vertex];

uvec3 offset(uint id) {
    uvec3 res = (id.xxx / uvec3(9*9, 9*9, 9)) * uvec3(1, 9, 9);
    res.z = id - res.z;
    res.y = id / 9 - res.y;
    return res;
}
/*
float hermite(vec3 pos) {
    
    vec3 res = normalize(pos - vec3(0.5));
    res *= 0.5;

    vec4 dsn = dsnoise(pos * 3.81419) * 8;
    dsn.yzw *= 3.81419;
    float sinpos = -sin(pos.x * 100 + dsn.x) * 0.01;
    res += sinpos * (dsn.yzw + vec3(100.0, 0.0, 0.0));

    return normalize(res);
}*/

float density(vec3 pos) {
    float sphere = distance(pos, vec3(0.5)) - 0.45;
    sphere *= 0.5;

    //sphere += snoise(pos * 510.52347) * 0.0003125;
    //sphere += snoise(pos * 257.98141) * 0.000625;
    //sphere += snoise(pos * 128.0141) * 0.00125;

    //sphere += snoise(pos * 64.7347) * 0.0025;
    //sphere += snoise(pos * 32.4124) * 0.005;
    sphere += cos(pos.x * 100 + snoise(pos * 3.81419) * 8) * 0.01;

    //sphere += snoise(pos * 8.12347) * 0.02;
    //sphere += snoise(pos * 4.98141) * 0.04;
    return sphere;
}

/*
float density(vec3 pos) {
    float sphere = distance(pos, vec3(0.5)) - 0.45;
    return sphere + sin(pos.x * 32.0 + time * 5) / 80.0 + sin(pos.y * 80.0 + time * 10) / 160.0 + sin(pos.z * 256.0 + time * 20) / 320.0;
}
*/

void addVertex(uvec3 id, vec3 position) {
    SSBO_position[id.x * size.y * size.z + id.y * size.z + id.z] = vec4(position, 0.0);
}

void addQuad(ivec3 a, ivec3 b, ivec3 c, ivec3 d) {
    uvec2 global_mult = uvec2(size.y * size.z, size.z);

    uint i = atomicCounterIncrement(tri_cnt);
    uvec4 indices = uvec4(a.x, b.x, c.x, d.x) * global_mult.x + 
                    uvec4(a.y, b.y, c.y, d.y) * global_mult.y + 
                    uvec4(a.z, b.z, c.z, d.z);

    SSBO_index[i * 3 + 0] = indices.xy;
    SSBO_index[i * 3 + 1] = indices.zx;
    SSBO_index[i * 3 + 2] = indices.wz;
}

void main() {
    vec3 pos = vec3(gl_WorkGroupID * gl_WorkGroupSize + offset(gl_LocalInvocationIndex));
    density_cache[gl_LocalInvocationIndex] = density(pos / vec3(size));

    if (gl_LocalInvocationIndex + local_size < local_size_vertex) {
        pos = vec3(gl_WorkGroupID * gl_WorkGroupSize + offset(gl_LocalInvocationIndex + local_size));
        density_cache[gl_LocalInvocationIndex + local_size] = density(pos / vec3(size));
    }

    memoryBarrierShared();
    barrier();

    pos = vec3(gl_LocalInvocationID);

    vec4 d_z0 = vec4(
        density_cache[uint(dot(pos                      , density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 0.0, 0.0), density_mult))],
        density_cache[uint(dot(pos + vec3(0.0, 1.0, 0.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 1.0, 0.0), density_mult))]
    );

    vec4 d_z1 = vec4(
        density_cache[uint(dot(pos + vec3(0.0, 0.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 0.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(0.0, 1.0, 1.0), density_mult))],
        density_cache[uint(dot(pos + vec3(1.0, 1.0, 1.0), density_mult))]
    );

    vec4 d_y0 = vec4(d_z0.xy, d_z1.xy);
    vec4 d_y1 = vec4(d_z0.zw, d_z1.zw);

    vec4 d_x0 = vec4(d_z0.xz, d_z1.xz);
    vec4 d_x1 = vec4(d_z0.yw, d_z1.yw);

    vec4 signs_x = step(d_x0 * d_x1, vec4(0.0));
    vec4 signs_y = step(d_y0 * d_y1, vec4(0.0));
    vec4 signs_z = step(d_z0 * d_z1, vec4(0.0));

    float cnt = dot(signs_x, vec4(1.0)) + dot(signs_y, vec4(1.0)) + dot(signs_z, vec4(1.0));

    if (cnt > 0.5) {
        vec4 alpha_x = clamp(d_x0 / (d_x0 - d_x1), 0.0, 1.0);
        vec4 alpha_y = clamp(d_y0 / (d_y0 - d_y1), 0.0, 1.0);
        vec4 alpha_z = clamp(d_z0 / (d_z0 - d_z1), 0.0, 1.0);

        alpha_x *= signs_x;
        alpha_y *= signs_y;
        alpha_z *= signs_z;

        vec3 position = vec3(
            dot(alpha_x, vec4(1.0)) + dot(vec4(signs_y.yw, signs_z.yw), vec4(1.0)),
            dot(alpha_y, vec4(1.0)) + dot(vec4(signs_x.yw, signs_z.zw), vec4(1.0)),
            dot(alpha_z, vec4(1.0)) + dot(vec4(signs_x.zw, signs_y.zw), vec4(1.0))
        ) / cnt;

        addVertex(gl_GlobalInvocationID, (vec3(gl_GlobalInvocationID) + position) / vec3(size));

        ivec3 ipos = ivec3(gl_GlobalInvocationID);

        if (signs_x.x == 1.0 && ipos.y * ipos.z != 0) addQuad(
            ipos, 
            ipos + ivec3(0, -1, 0),
            ipos + ivec3(0, -1, -1),
            ipos + ivec3(0, 0, -1)
        );

        if (signs_y.x == 1.0 && ipos.x * ipos.z != 0) addQuad(
            ipos, 
            ipos + ivec3(-1, 0, 0),
            ipos + ivec3(-1, 0, -1),
            ipos + ivec3(0, 0, -1)
        );

        if (signs_z.x == 1.0 && ipos.x * ipos.y != 0) addQuad(
            ipos, 
            ipos + ivec3(-1, 0, 0),
            ipos + ivec3(-1, -1, 0),
            ipos + ivec3(0, -1, 0)
        );
    }
}
