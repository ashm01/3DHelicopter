
#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Shader;

const int NumberOfVertexCoords = 24;
const int NumberOfTriangleIndices = 36;

class Cube
{
private:
	float dim;
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
	GLuint ibo;                     //identifier for the triangle indices
	
	static int numOfVerts;
	static int numOfTris;
	
	float verts[NumberOfVertexCoords];
	float cols[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];
public:
	
	Cube();
	glm::mat4 ModelViewMatrix;
	glm::vec3 cubePos;
	void setDim(float d);
	void constructGeometry(Shader* myShader);
	void render();
	//glm::vec3 getCubePos();
};