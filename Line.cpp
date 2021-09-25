#include "Line.h"

void Line::draw(ShaderProgram* sp, glm::mat4 M )
{
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, coords); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("colors")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("colors"), 4, GL_FLOAT, false, 0, color); //Specify source of the data for the attribute vertex

	glDrawArrays(GL_LINES, 0, 2); //Draw the object

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("colors"));
}

Line::Line(float* _coords, float* _color):coords(_coords),color(_color)
{
	
}
