#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "lodepng.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

namespace fs = std::filesystem;

static GLuint* loadTexture(std::string fileName,int i) {
	GLuint* tex = new GLuint;
	glActiveTexture(GL_TEXTURE0+i);
	//Read into computers memory
	std::vector<unsigned char> image; //Allocate memory
	unsigned width, height; //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, fileName);
	//Import to graphics card memory
	glGenTextures(1, tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, *tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


static std::map<std::string, GLuint*> loadTexturesdir() {
	std::string path = "Assets";
	std::map<std::string, GLuint*> out;
	int itr = 0;
	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.path().extension().string() == ".png") {
			out.insert(std::pair<std::string, GLuint*>(entry.path().stem().string(), loadTexture(entry.path().string(),itr)));
			++itr;
		}
	}
	return out;
}