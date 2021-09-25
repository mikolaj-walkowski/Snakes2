#pragma once
#include "DynamicEntity.h"
class SnakeBase : public DynamicEntity
{
public:
	float maxHp = 5;
	float hp = 5;
	bool team;
	glm::vec3 shootOffset = glm::vec3(0,0.6,0);

	void react(AbstractEngine& ab);

	SnakeBase();
	SnakeBase(ShaderProgram* _sp, GLuint* _tex, bool _team, Mesh* objectGeometry, Mesh* colisiongeometry, glm::vec3 center, glm::quat rotation);
	~SnakeBase();
};

