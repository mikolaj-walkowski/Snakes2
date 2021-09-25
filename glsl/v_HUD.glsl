#version 330

uniform mat4 P;
uniform vec4 color;

in vec2 vertex;

out vec4 iColor;

void main(void) {
	iColor = color; 
	gl_Position = P*vec4(vertex,0,1);
}