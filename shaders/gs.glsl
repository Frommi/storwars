#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 gs_position[];
in vec2 gs_UV[];
in vec3 gs_normal[];
in vec3 gs_bari[];

out vec2 fs_UV;
out vec3 fs_normal;
out vec3 fs_bari;
out vec3 fs_tri;
out vec3 fs_position;

uniform mat4 u_VP;
uniform vec3 u_ObsPos;
uniform vec3 u_ObsImpulse;
         
uniform float u_InverseG;

vec3 getLorentz(int i) {
	vec3 r = -u_ObsPos - gs_position[i];
	return r + u_ObsImpulse * (length(r) + dot(r, u_ObsImpulse) * u_InverseG);
}

void main() {
    //vec3 normal = cross(gs_position[0] - gs_position[1], gs_position[0] - gs_position[2]);
    //normal = normalize(normal);

	for (int i = 0; i < 3; ++i) {
    	gl_Position = u_VP * vec4(getLorentz(i), 1.0);
    	fs_position = -u_ObsPos - gs_position[i];
	    fs_normal = gs_normal[i];
        //fs_normal = normal;
    	fs_UV = gs_UV[i];
    	fs_bari = gs_bari[i];
    	fs_tri = vec3(0.0 + int(i == 0), 0.0 + int(i == 1), 0.0 + int(i == 2));
	    EmitVertex();
	}
	EndPrimitive();
}
