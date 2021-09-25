#pragma once
#include <HitBox.h>
#include <Mesh.h>
#include <stdlib.h>
#include <utility>
#include <vector>

#include"AbstractEngine.h"


class Entity: AbstractEntity
{
public:
	ShaderProgram* sp;
	GLuint* tex;

	Mesh* objectGeometry;
	Mesh* collisonGeometry;

	float* collisionGemoetryCalculated;
	float collisonRadius;

	glm::vec4 center;
	glm::mat4 M;
	glm::quat rotation;
	glm::vec3 Scale = glm::vec3(1.0f);

	std::pair<int,int> isColliding(Entity* e);
	std::vector<int> everyFaceCollsion(Entity* e);
	bool isInCollisonRadius(Entity* e);
	virtual void draw();
	void drawBoundingBox();
	void moveModel();
	void calculateM();
	virtual glm::mat4 getM();
	virtual float* getCollisonModel();
	virtual void react(AbstractEngine& ab);

	Entity();
	Entity(ShaderProgram* _sp,GLuint* _tex,Mesh* _objGeo, glm::vec3 _center = glm::vec3(0, 0, 0),glm::quat _rot = glm::quat(1, 0, 0, 0));
	Entity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, Mesh* _objColl, glm::vec3 _center = glm::vec3(0, 0, 0), glm::quat _rot = glm::quat(1, 0, 0, 0));
	~Entity();
};

