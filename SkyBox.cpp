#include "SkyBox.h"

SkyBox::SkyBox(ShaderProgram* _sp, GLuint* _tex, Mesh* obj)
{
	sp = _sp;
	tex = _tex;
	objectGeometry = obj;
	center = glm::vec4(0, 0, 0, 1);
	rotation = glm::quat(1, 0, 0, 0);
	Scale = glm::vec3(60, 60, 60);
	calculateM();
}
