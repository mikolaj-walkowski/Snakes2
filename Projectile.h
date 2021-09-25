#pragma once
#include "DynamicEntity.h"
class Projectile : public DynamicEntity
{
public:
	float damage = 4;
	float range = 4; 
	float force = 20;

	virtual void react(AbstractEngine& ab);

	glm::mat4 getM() override;

	Projectile();
	Projectile(ShaderProgram* _sp, GLuint* _tex, Mesh* objectGeometry,Mesh* colisiongeometry,glm::vec3 center,glm::quat rotation);
	~Projectile();
};

