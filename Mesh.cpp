#include "Mesh.h"

void Mesh::draw(ShaderProgram* sp, glm::mat4 M,GLuint* tex) {
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, coords); 

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); 

	glEnableVertexAttribArray(sp->a("color"));
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); 

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, TexCoords);

	glUniform1i(sp->u("tex"),*tex);

	glDrawArrays(GL_TRIANGLES, 0, verticesNo); //Draw the object

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("texCoord"));

}
Mesh::~Mesh() {
	delete[] coords;
	delete[] TexCoords;
	delete[] normals;
	delete[] colors;
}
Mesh::Mesh(int _vertiecesNo) {
	verticesNo = _vertiecesNo;
	coords = new float[verticesNo * 4];
	TexCoords = new float[verticesNo * 2];
	normals = new float[verticesNo * 4];
	colors = new float[verticesNo * 4];
}
void Mesh::setColors(float _coll[]) {
	for (int i = 0; i < verticesNo; i++)
	{
		colors[i*4] = _coll[0];
		colors[i*4+1] = _coll[1];
		colors[i*4+2] = _coll[2];
		colors[i*4+3] = _coll[3];
	}
}