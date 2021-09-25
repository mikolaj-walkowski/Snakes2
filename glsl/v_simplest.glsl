#version 330
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lightSource;
uniform vec4 camPos;

in vec4 vertex;
in vec4 color;
in vec4 normal; 
in vec2 texCoord;

out vec2 iTexCoord;

out vec4 ic;
out vec4 l;
out vec4 n;
out vec4 v;

void main(void) {
	l = normalize(lightSource);
	n = normalize(M*normal - M*vec4(0,0,0,1));
	v = normalize(camPos- M*vertex);

	iTexCoord = texCoord;

	ic = color;

	gl_Position = P*V*M*vertex;
}
