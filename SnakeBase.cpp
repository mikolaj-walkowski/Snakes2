#include "SnakeBase.h"

SnakeBase::SnakeBase(){
	this->mass = 5;
	this->canBeDamaged = true;
}

SnakeBase::SnakeBase(ShaderProgram* _sp, GLuint* _tex, bool _team, Mesh* objectGeometry, Mesh* colisiongeometry, glm::vec3 center, glm::quat rotation) : team(_team), DynamicEntity(_sp, _tex, objectGeometry, colisiongeometry, center, rotation)
{
	
	this->mass = 5;
	this->canBeDamaged = true;
	this->Scale = glm::vec3(0.3f);//glm::vec3(0.1f, 0.1f, 0.1f);
	this->calculateM();
	this->moveModel();
}

SnakeBase::~SnakeBase()
{
}

void SnakeBase::react(AbstractEngine& ab) {
	ab.react(this);
}