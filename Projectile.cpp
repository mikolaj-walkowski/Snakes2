#include "Projectile.h"
#include "SnakeBase.h"

Projectile::Projectile()
{
	this->mass = 0.5f;
	this->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
}

Projectile::Projectile(ShaderProgram* _sp, GLuint* _tex, Mesh* objectGeometry, Mesh* colisiongeometry, glm::vec3 center, glm::quat rotation): DynamicEntity(_sp, _tex, objectGeometry, colisiongeometry, center, rotation)
{
	this->mass = 0.5f;
	this->simulateWind = true;
	this->Scale = glm::vec3(1.0);//glm::vec3(0.1f, 0.1f, 0.1f);
	this->calculateM();
	this->moveModel();
}

Projectile::~Projectile()
{
}

glm::mat4 Projectile::getM(){
	if (wasMoved) {
		auto norm = glm::normalize(velocity);
		rotation = glm::quat(glm::vec3(1.f,0.f,0.f), norm);
		calculateM();
		moveModel();
		wasMoved = false;
	}
	return this->M;
}
void Projectile::react(AbstractEngine& ab) {
	ab.react(this);
}
