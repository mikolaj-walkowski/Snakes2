#include "Game.h"
#include "OBJloader.h"
#include "TextureLoader.h"

Game::Game(std::map<std::string, float>* in):input(in)
{
	this->meshLibrary = loadOBJdir();
	this->textureLibrary = loadTexturesdir();
	Ground = new Entity(sp_regular,textureLibrary["noTex"], meshLibrary["flats"]);
	world.push_back(Ground);
	statEnt.push_back(Ground);
	float eh[] = { 0.0f,1.f,0.f,0 };
	Ground->objectGeometry->setColors(eh);

	skyBox = new SkyBox(sp_sky,textureLibrary["SkyBox2"], meshLibrary["Skycube"]);
	world.push_back(skyBox);
	//meshLibrary["lesnek"], meshLibrary["lesnekHitbox"] meshLibrary["cube"], meshLibrary["cube"]
	spawn(new SnakeBase(sp_regular, textureLibrary["noTex"], 1, meshLibrary["lesnek"], meshLibrary["lesnekHitbox"], glm::vec3(0, 5, 0), glm::quat(1,0,0,0)));
	spawn(new SnakeBase(sp_regular, textureLibrary["noTex"], 1 , meshLibrary["lesnek"], meshLibrary["lesnekHitbox"], glm::vec3(0, 5, -4),glm::quat(1,0,0,0)));
	spawn(new SnakeBase(sp_regular, textureLibrary["noTex"], 0 , meshLibrary["lesnek"], meshLibrary["lesnekHitbox"], glm::vec3(-4, 5, -4), glm::quat(1,0,0,0)));
	spawn(new SnakeBase(sp_regular, textureLibrary["noTex"], 0, meshLibrary["lesnek"], meshLibrary["lesnekHitbox"], glm::vec3(-4, 5, 0), glm::quat(1,0,0,0)));
	//spawn(new DynamicEntity(meshLibrary["cube"], glm::vec3(0, 5, 0)));
	//spawn(new DynamicEntity(meshLibrary["cube"], glm::vec3(0, 5, -4)));
	player = redTeam[0];
	physicsEngine = new PhysicsEngine(this);
}

Game::~Game()
{
	for (auto itr : meshLibrary) {
		delete itr.second;
	}
	for (auto itr : textureLibrary) {
		glDeleteTextures(1,itr.second);
		delete itr.second;
	}
	for (auto itr : world) {
		delete itr;
	}
	delete physicsEngine;
	delete sp_regular;
	delete sp_sky;
	delete sp_HUD;
}

void Game::spawn(DynamicEntity* de)
{	
	world.push_back(de);
	dynEnt.push_back(de);
	float eh[] = { 0.0f,1.f,0.f,0 };
	de->objectGeometry->setColors(eh);
}

void Game::spawn(SnakeBase* de)
{
	world.push_back(de);
	dynEnt.push_back(de);
	if (de->team)
		redTeam.push_back(de);
	else
		blueTeam.push_back(de);
	float eh[] = { 0.0f,1.f,0.f,0 };
	de->objectGeometry->setColors(eh);
}

void Game::deleteEnt(DynamicEntity* e) {
	for (int i = 0; i < world.size(); i++)
	{
		if (e == world[i]) {
			world.erase(world.begin() + i);
			break;
		}
	}
	for (int i = 0; i < dynEnt.size(); i++)
	{
		if (e == dynEnt[i]) {
			dynEnt.erase(dynEnt.begin() + i);
			break;
		}
	}
	delete e;
}

void Game::deleteEnt(Entity* e) {
	for (int i = 0; i <world.size(); i++)
	{
		if(e == world[i]){
			world.erase(world.begin()+i);
			break;
		}
	}
	for (int i = 0; i < statEnt.size(); i++)
	{
		if (e == statEnt[i]) {
			statEnt.erase(statEnt.begin() + i);
			break;
		}
	}
	delete e;
}

void Game::deleteGamer(DynamicEntity* de) {
	SnakeBase* e = (SnakeBase*)de;
	if (e->team) {
		for (int i = 0; i < redTeam.size(); i++){
			if (e == redTeam[i]) {
				redTeam.erase(redTeam.begin() + i);
				break;
			}
		}
		if (redTeam.size() == 0) {
			printf("BLUE wins");
			end = true;
			return;
		}
		if (player == e) {
			player = redTeam[0];
			currPlayer = 0;
		}
	}
	else {
		for (int i = 0; i < blueTeam.size(); i++){
			if (e == blueTeam[i]) {
				blueTeam.erase(blueTeam.begin() + i);
				break;
			}
		}
		if (blueTeam.size() == 0) {
			printf("RED wins");
			end = true;
			return;
		}
		if (player == e) {
			player = blueTeam[0];
			currPlayer = 0;
		}
	}	
	deleteEnt(e);
}


void Game::explosion(glm::vec4 center, float force,float damage, float range) {
	float dist;
	for (int i = 0; i < dynEnt.size(); i++)
	{
		auto de = dynEnt[i];
		if (center != de->center) {
			dist = glm::distance(de->center, center);
			if (dist <= range) {
				float a_cof = (force / de->mass) / (glm::pow(dist, 2));
				auto tmp = glm::vec3(a_cof * glm::normalize(de->center - center));
				dynEnt[i]->velocity += tmp;
				if (de->canBeDamaged)
					((SnakeBase*)de)->hp -= (float)((force / de->mass) / (glm::pow(dist, 2)));
			}
		}
	}
}

float theta=3.14/2, gamma=0, r = 3 ;

glm::vec3 inputVelo(0,0,0);

void Game::processInput(float& dT)
{
	if (player == NULL) { this->V = glm::lookAt(cameraPos, cameraFront, cameraUp); inputVelo *= 0; return; }//TODO zrób freecam
	
	if ((*input)["Select"] != 0) {
		if (currTeam) {
			currPlayer += (*input)["Select"];
			if (currPlayer < 0)
				currPlayer += redTeam.size();
			else
				currPlayer %= redTeam.size();
			player = redTeam[currPlayer];
		}
		else {
			currPlayer += (*input)["Select"];
			if (currPlayer < 0)
				currPlayer += blueTeam.size();
			else
				currPlayer %= blueTeam.size();
			player = blueTeam[currPlayer];
		}
		(*input)["Select"] = 0;
	}
	theta = theta - (*input)["Yoffset"];

	theta = glm::clamp(theta, 0.1f, 3.13f);


	cameraPos = glm::vec3(0,glm::cos(theta),1 - glm::abs(glm::cos(theta)))*r;
	cameraPos = player->rotation * cameraPos +glm::vec3(player->center);
	
	cameraFront = glm::vec3(player->center);// -cameraPos;
	
	player->rotation = glm::rotate(player->rotation, -(*input)["Xoffset"],glm::vec3(0,1,0));
	player->wasMoved = true;

	glm::vec3 playerRight = glm::normalize(glm::cross(glm::normalize(cameraFront - cameraPos), cameraUp));
	glm::vec3 playerFront = glm::normalize(glm::cross(cameraUp, playerRight));

	inputVelo = playerRight * (*input)["Xmove"] + playerFront * (*input)["Zmove"];

	if ((*input)["shoot"] != 0) {
		physicsEngine->shoot(glm::normalize(glm::vec3(player->center) - cameraPos), glm::vec3(player->center)+player->shootOffset);
		//timer = turnTime;
		(*input)["shoot"] = 0;
	}

	player->velocity += inputVelo;

	if (player->wasOnGround && (*input)["Ymove"] > 0) {
		
		player->velocity.y = 5;
	}		
	//Przesuniêcie na prawo i do góry 
	cameraPos += glm::vec3(player->getM() * player->cameraOffset - player->getM() * glm::vec4(0, 0, 0, 1));
	cameraFront += glm::vec3(player->getM() * player->cameraOffset - player->getM() * glm::vec4(0, 0, 0, 1));

	//printf("Player velo = %f, %f, %f\n",player->velocity.x, player->velocity.y, player->velocity.z);
	this->V = glm::lookAt(cameraPos, cameraFront, cameraUp);
	
}

void Game::drawHUD()
{
	sp_HUD->use();
	glUniformMatrix4fv(sp_HUD->u("P"), 1, false, glm::value_ptr(P));
	
	DrawQuad(0,0, 1000, 200, glm::vec4(0.10, 0.14, 0.04, 0));

	//Time
	DrawQuad(100, 50,100 , 20, glm::vec4(1, 1, 1, 1));
	DrawQuad(100, 50, (1-(timer/ turnTime))*100, 20, glm::vec4(0, 1, 0, 1));

	//CurrPlayer hp 

	DrawQuad(100, 100, 100, 20, glm::vec4(1, 1, 1, 1));
	DrawQuad(100, 100, (player->hp / player->maxHp) * 100, 20, glm::vec4(1, 0, 0, 1));

	//Team 

	if(player->team)
		DrawQuad(15, 50, 70, 70, glm::vec4(1, 0, 0, 1));
	else
		DrawQuad(15, 50, 70, 70, glm::vec4(0, 0, 1, 1));
	
}

void Game::DrawQuad(float x,float y, float width,float height, glm::vec4 color) {

	float coords[] = {
		x,y,
		x + width,y,
		x + width,y + height,
		x,y,
		x,y + height,
		x + width,y + height, };

	glUniform4fv(sp_HUD->u("color"), 1, glm::value_ptr(color));

	glEnableVertexAttribArray(sp_HUD->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp_HUD->a("vertex"), 2, GL_FLOAT, false, 0, coords); //Specify source of the data for the attribute vertex
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(sp_HUD->a("vertex"));
	
}


void Game::proccesFrame(float& dT)
{
	processInput(dT);

	//move
	for (auto itr : dynEnt)
	{
		physicsEngine->applyForces(*itr,dT);
	}
	//check for collsions
	std::vector<int> tmp;
	float green[] = {0,1,0,0};
	DynamicEntity* itr;
	for (int i = 0; i < dynEnt.size(); ++i) {
		itr = dynEnt[i];
		itr->wasOnGround = false;
		for (int j = i + 1; j < dynEnt.size(); ++j) {
			if (itr->isInCollisonRadius(dynEnt[j])) {
				itr->getCollisonModel();
				dynEnt[j]->getCollisonModel();
				tmp = itr->everyFaceCollsion(dynEnt[j]);
				if (tmp.size() != 0) {
					physicsEngine->collision(itr, dynEnt[j], tmp);
				}
			}
		}
		for (auto wE : statEnt) {
			wE->objectGeometry->setColors(green);
			if (itr->isInCollisonRadius(wE)) {
				itr->getCollisonModel();
				wE->getCollisonModel();
				tmp = itr->everyFaceCollsion(wE);
				if (tmp.size() != 0) {
					physicsEngine->collision(itr, wE, tmp);
					if (Ground == wE) {
						itr->wasOnGround = true;
					}
				}
			}
		}
	}
	for (auto itr2 : dynEnt)
	{
		if (outOfBoundsCheck(itr2))
			continue;
		itr2->move(dT);
	}

	if(inputVelo != glm::vec3(0,0,0))
		player->velocity -= glm::dot(player->velocity, glm::normalize(inputVelo) )* glm::normalize(inputVelo);
	for (auto red : redTeam) {
		if (outOfBoundsCheck(red))
			continue;
		if (red->hp <= 0)
			deleteGamer(red);
		//printf("red hp:%f/5\n", ((SnakeBase*)red)->hp);
	}
	for (auto blue : blueTeam) {
		if (outOfBoundsCheck(blue))
			continue;
		if (blue->hp <= 0)
			deleteGamer(blue);
		//printf("blue hp:%f/5\n", ((SnakeBase*)blue)->hp);
	}
	for (auto itr2 : dynEnt)
	{
		outOfBoundsCheck(itr2);
	}
	timer += dT;
	if (timer > turnTime) {
		timer = 0;
		currTeam=!currTeam;
		if (currTeam) {
			player = redTeam[0];
		}
		else
			player = blueTeam[0];
		currPlayer = 0;
	}
}

bool Game::boundMachinery(Entity* e) {
	if (glm::abs(e->center.x) >= limit)
		return true;
	if (glm::abs(e->center.y) >= limit)
		return true;
	if (glm::abs(e->center.z) >= limit)
		return true;
	return false;
}
bool Game::outOfBoundsCheck(DynamicEntity* e) {
	if (boundMachinery(e)) {
		deleteEnt(e);
		return true;
	}
	return false;
}
bool Game::outOfBoundsCheck(SnakeBase* sb) {
	if (boundMachinery(sb)) {
		deleteGamer(sb);
		return true;
	}
	return false;
}
