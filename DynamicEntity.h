#pragma once
#include "Entity.h"

class DynamicEntity: public Entity
{
public:
	glm::vec4 cameraOffset = glm::vec4(2.5, 1.5, 0, 1);
	float mass= 2;
	glm::vec3 velocity = glm::vec3(0.0f);

	bool simulateWind = 0;
	bool simulateGravity = 1;
	bool wasOnGround = false;
	bool wasMoved = false;
	bool canBeDamaged = false;

	void colideResolve(Entity e);
	bool isSimulateWind();
	bool isSimulateGravity();
	void setSimulateWind(bool flag);
	void setSimulateGravity(bool flag);
	void move(const float& dT);
	virtual glm::mat4 getM();
	float* getCollisonModel();
	

	~DynamicEntity();
	DynamicEntity();
	DynamicEntity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, Mesh* _objColl, glm::vec3 _center = glm::vec3(0, 0, 0), glm::quat _rot = glm::quat(1, 0, 0, 0));
	DynamicEntity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, glm::vec3 _center = glm::vec3(0, 0, 0), glm::quat _rot = glm::quat(1, 0, 0, 0));

	virtual void react(AbstractEngine& ab);
};