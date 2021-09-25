#version 330
uniform sampler2D tex;

out vec4 pixelColor;

in vec4 iColor;
in vec2 iTexCoord;

void main(void) {
	pixelColor = texture(tex,iTexCoord);
}