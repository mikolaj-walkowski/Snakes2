#pragma once

#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Mesh.h"

namespace fs = std::filesystem;

static Mesh* loadOBJ(std::string fileName) {
	std::ifstream input(fileName);
	std::stringstream ss;
	std::string s;
	std::string pref;
	
	std::vector<glm::vec4> coords;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec4> normals;

	glm::vec3 tmpvec3;
	glm::vec2 tmpvec2;
	GLint tmpGLint;

	std::vector<GLint> coordsIndicies;
	std::vector<GLint> texCoordsIndicies;
	std::vector<GLint> normalsIndicies;

	Mesh* output;


	if (!input.is_open())
		throw "Error::OBJloader::Cannot open file: " + fileName;
	while (std::getline(input, s)) {
		ss.clear();
		ss.str(s);
		ss >> pref;
		if (pref == "v") {
			ss >> tmpvec3.x >> tmpvec3.y >> tmpvec3.z;
			coords.push_back(glm::vec4(tmpvec3,1));
		}
		else if(pref=="vt") {
			ss >> tmpvec2.x >> tmpvec2.y;
			texCoords.push_back(tmpvec2);
		}
		else if (pref == "vn") {
			ss >> tmpvec3.x >> tmpvec3.y >> tmpvec3.z;
			normals.push_back(glm::vec4(tmpvec3, 1));
		}
		else if (pref == "f") {

			for (int i = 0; i < 3; i++)
			{
				ss >> tmpGLint;
				coordsIndicies.push_back(tmpGLint);
				ss.ignore(1, '/');
				ss >> tmpGLint;
				texCoordsIndicies.push_back(tmpGLint);
				ss.ignore(1, '/');
				ss >> tmpGLint;
				normalsIndicies.push_back(tmpGLint);
			}
		}else{}
	}
	output = new Mesh(coordsIndicies.size());
	for (int i = 0; i < coordsIndicies.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			output->coords[i * 4 + j] = coords[coordsIndicies[i] - 1][j];
		}
		std::cout << '\n';
		for (int j = 0; j < 2; j++)
		{
			output->TexCoords[i * 2 + j] = texCoords[texCoordsIndicies[i] - 1][j];
		}
		for (int j = 0; j < 4; j++)
		{
			output->normals[i * 4 + j] = normals[normalsIndicies[i] - 1][j];
		}
	}

	return output;
}

static std::map<std::string, Mesh*> loadOBJdir() {
	std::string path = "OBJfiles";
	std::map<std::string, Mesh*> out;
	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.path().extension().string() == ".obj") {
			out.insert(std::pair<std::string,Mesh*>(entry.path().stem().string(), loadOBJ(entry.path().string())));
		}
	}
	return out;
}