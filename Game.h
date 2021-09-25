#pragma once
#include <vector>
#include <string>
#include <map>
#include "shaderprogram.h"

#include "constants.h"

#include "Entity.h"
#include "DynamicEntity.h"

#include "PhysicsEngine.h"
#include "Line.h"
#include "SkyBox.h"

class PhysicsEngine;

class Game
{
private:
	PhysicsEngine *physicsEngine;
public:
	ShaderProgram* sp_regular = new ShaderProgram("glsl/v_simplest.glsl", NULL, "glsl/f_simplest.glsl");
	ShaderProgram* sp_sky = new ShaderProgram("glsl/v_skybox.glsl", NULL, "glsl/f_skybox.glsl");
	ShaderProgram* sp_HUD = new ShaderProgram("glsl/v_HUD.glsl", NULL, "glsl/f_HUD.glsl");

	float limit = 50;
	bool end = false;
	float timer=0,turnTime =30;

	std::map<std::string, float>* input;
	
	std::vector<Entity*> debug;
	std::vector<Line*> debugLines;

	std::vector<SnakeBase*> blueTeam;
	std::vector<SnakeBase*> redTeam;

	std::vector<Entity*> world;
	std::vector<Entity*> statEnt;
	std::vector<DynamicEntity*> dynEnt;

	std::map<std::string, Mesh*> meshLibrary;
	std::map<std::string, GLuint*> textureLibrary;

	glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 P = glm::ortho(0.f, 1000.f, 1000.f, 0.f, -1.f, 1.f);

	SnakeBase* player;
	Entity* Ground;
	SkyBox* skyBox;

	bool currTeam=true;
	int currPlayer = 0;
	
	glm::mat4 V;

	Game(std::map<std::string, float>* in);
	~Game();

	void drawHUD();
	void DrawQuad(float x, float y, float width, float height, glm::vec4 color);
	void prepHud();

	void spawn(DynamicEntity* de);
	void spawn(SnakeBase* s);
	void proccesFrame(float& dT);
	void deleteEnt(Entity* e);
	void deleteEnt(DynamicEntity* e);
	void deleteGamer(DynamicEntity* e);

	void processInput(float& dT);

	void explosion(glm::vec4 center, float force,float damage, float range);

	bool boundMachinery(Entity* e);
	bool outOfBoundsCheck(DynamicEntity* e);
	bool outOfBoundsCheck(SnakeBase* sb);
};

