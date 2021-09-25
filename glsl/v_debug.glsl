#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec4 color;
in int cool;

out vec4 iColor;

void main(void) {
	if(cool == 1 )
		iColor = vec4(1,0,0,1);
	else 
		iColor = vec4(0,0,1,1);
	gl_Position = P*V*M*vertex;
}