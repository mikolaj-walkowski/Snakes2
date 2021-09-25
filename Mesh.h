#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"

class Mesh {
public:
	int verticesNo;
	float* coords;
	float* TexCoords;
	float* normals;
	float* colors;
	void draw(ShaderProgram* sp, glm::mat4 M, GLuint* tex);
	void setColors(float _coll[]);
	~Mesh();
	Mesh(int _vertiecesNo);
};