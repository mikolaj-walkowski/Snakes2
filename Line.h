#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>

class Line
{
public:
	float *coords;
	float *color;
	void draw(ShaderProgram* sp, glm::mat4 M = glm::mat4(1.0f));
	Line(float* _coords, float* _color);
};

