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

void main(void) {
	iTexCoord = texCoord;

	gl_Position = P*V*M*vertex;
}