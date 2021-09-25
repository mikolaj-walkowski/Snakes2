#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "AbstractEngine.h"

#include "Entity.h"
#include "DynamicEntity.h"
#include "Projectile.h"
#include "SnakeBase.h"

#include <vector>
#include <map>
#include <string>

class Game;

class PhysicsEngine:AbstractEngine
{
public:
	glm::vec3 windSpeed = glm::vec3(1.f,0.f,0.f);
	float g = 7;
	float energyLost=0.10;
	Game* game;

	PhysicsEngine(Game* _game);

	void collision(DynamicEntity* de1, DynamicEntity* de2,std::pair<int,int> coll);
	void collision(DynamicEntity* de1, DynamicEntity* de2, std::vector<int>& coll);
	void collision(DynamicEntity* de, Entity* e, std::pair<int,int> coll);
	void collision(DynamicEntity* de, Entity* e, std::vector<int>& coll);

	void react(Projectile* p);
	void react(SnakeBase* sb);
	void react(Entity* e);
	void react(DynamicEntity* de);

	void applyForces(DynamicEntity& de,float dT);

	void shoot(glm::vec3 direction, glm::vec3 center);
};

