#include "DynamicEntity.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
void DynamicEntity::colideResolve(Entity e)
{
}

bool DynamicEntity::isSimulateWind()
{
	return simulateWind;
}

bool DynamicEntity::isSimulateGravity()
{
	return simulateGravity;
}

void DynamicEntity::setSimulateWind(bool flag)
{
	this->simulateWind = flag;
}

void DynamicEntity::setSimulateGravity(bool flag)
{
	this->simulateGravity = flag;
}

void DynamicEntity::move(const float& dT)
{
	
	center += glm::vec4(velocity * dT, 0);
	wasMoved = true;
}


glm::mat4 DynamicEntity::getM()
{
	if (wasMoved) {
		calculateM();
		moveModel();
		wasMoved = false;
	}
	return this->M;
}

float* DynamicEntity::getCollisonModel()
{
	if (wasMoved) {
		calculateM();
		moveModel();
		wasMoved = false;
	}
	return collisionGemoetryCalculated;
}

DynamicEntity::~DynamicEntity()
{
}

DynamicEntity::DynamicEntity():Entity()
{
	
}

DynamicEntity::DynamicEntity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, Mesh* _objColl,glm::vec3 _center,glm::quat _rot):Entity(_sp,_tex,_objGeo,_objColl, _center, _rot)
{
}

DynamicEntity::DynamicEntity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, glm::vec3 _center,glm::quat _rot):Entity(_sp,_tex,_objGeo, _center,_rot)
{
}

void  DynamicEntity::react(AbstractEngine& ab) {
	ab.react(this);
}
