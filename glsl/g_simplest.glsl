#version 330

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float amount;

in vec4 gColor[];
in vec4 gVertex[];

out vec4 iColor;
 
void main(void) {
	int i;
	vec4 normal = amount*normalize(vec4(cross(gVertex[2].xyz-gVertex[0].xyz,gVertex[1].xyz-gVertex[0].xyz),0));
	for (i = 0; i < gl_in.length(); i++) {
		gl_Position = P*V*M*(gVertex[i]+normal);
		iColor= gColor[i];
		EmitVertex();
	}
	EndPrimitive();
}
