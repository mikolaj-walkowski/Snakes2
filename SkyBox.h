#pragma once
#include "Entity.h"
class SkyBox:public Entity
{
public:

	SkyBox(ShaderProgram* _sp, GLuint* _tex, Mesh* obj);

};

