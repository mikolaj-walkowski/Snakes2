#include "PhysicsEngine.h"


PhysicsEngine::PhysicsEngine(Game* _game):game(_game)
{
}

void PhysicsEngine::collision(DynamicEntity* de1, DynamicEntity* de2,std::pair<int,int> coll)
{
		
}

void PhysicsEngine::collision(DynamicEntity* de1, DynamicEntity* de2, std::vector<int>& coll)
{
	glm::vec3 n = glm::normalize(glm::vec3(de1->center - de2->center));
	
	float v1 = glm::dot(de1->velocity, n);
	de1->velocity -= v1 * n;
	
	float v2 = glm::dot(de2->velocity, n);
	de2->velocity -= v2 * n;
	
	v1 = (de1->mass * v1 + de2->mass * v2) / (de1->mass+ de2->mass);
	
	de1->velocity += v1 * n;
	de2->velocity += v1 * n;

	de1->react(*this);
	de2->react(*this);
}

void PhysicsEngine::collision(DynamicEntity* de, Entity* e,std::pair<int,int> coll)
{
	auto sN = e->collisonGeometry->normals;
	glm::vec4 cN(sN[coll.second * 12], sN[coll.second * 12+1], sN[coll.second * 12+2],1);
	cN = e->M*cN;
	glm::vec3 cN3(cN);
	cN3 = glm::normalize(cN3);
	de->velocity -= cN3*glm::dot(cN3,de->velocity);
	float amount = glm::dot(cN3, de->velocity);
	if (amount < 0)
		de->velocity -= cN3 * amount;
	de->react(*this);
	e->react(*this);
}
void PhysicsEngine::collision(DynamicEntity* de, Entity* e, std::vector<int>& coll)
{
	auto sN = e->collisonGeometry->normals;
	for (int i = 1; i < coll.size(); i += 2) {
		glm::vec4 cN(sN[coll[i] * 12], sN[coll[i] * 12 + 1], sN[coll[i] * 12 + 2], 1);
		cN = e->M * cN;
		glm::vec3 cN3(cN);
		cN3 = glm::normalize(cN3);
		float amount = glm::dot(cN3, de->velocity);
		if(amount< 0 )
			de->velocity -= cN3 * amount;
	}
	de->react(*this);
	e->react(*this);
}

void PhysicsEngine::react(Projectile* p)
{
	game->explosion(p->center,p->force,p->damage,p->range);
	game->deleteEnt(p);
	//printf("p\n");
}

void PhysicsEngine::react(SnakeBase* sb)
{
	//printf("sb\n");
}

void PhysicsEngine::react(Entity* e)
{
	//printf("e\n");
}

void PhysicsEngine::react(DynamicEntity* de)
{
	//printf("de\n");
}


void PhysicsEngine::applyForces(DynamicEntity& de,float dT)
{
	if (de.simulateWind) {
		de.velocity += (windSpeed / de.mass)*dT; 
	}
	if (de.simulateGravity) {
		de.velocity.y -= g * dT;
	}
	if (de.wasOnGround&&glm::length(de.velocity)<3) {
		de.velocity *= 0;
	}
	de.velocity *= (1- energyLost * dT);
}

void PhysicsEngine::shoot(glm::vec3 direction, glm::vec3 center) {
	glm::quat rot = glm::quat(glm::vec3(1, 0, 0),direction);
	//printf("dir: %f, %f, %f\n", direction.x, direction.y, direction.z);
	center = center + 2.f*direction;
	auto tmp = new Projectile(game->sp_regular,game->textureLibrary["noTex"], game->meshLibrary["projectile"], game->meshLibrary["projectileHitbox"], center, rot);
	tmp->velocity += direction * 10.f;
	game->spawn(tmp);
	//printf("%f,%f,%f \n",direction.x,direction.y,direction.z);
}

