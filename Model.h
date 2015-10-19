#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "shaders/Shader.h"



class Model
{
private:

	ThreeDModel threeDModel;
	OBJLoader ModelObjLoader;
	

public:
	BoundingBox BB;
	double modelRadious;
	//Material properties
	glm::vec4 Model_Ambient;
	glm::vec4 Model_Diffuse;
	glm::vec4 Model_Specular;
	float Model_Shininess;
	Model();
	glm::mat4 ModelViewMatrix;
	glm::vec3 position;
	glm::vec3 boxDimensions;
	void render(glm::mat4 ViewMatrix);
	void loadModel(Shader &myShader, char *filename, glm::vec3 position, 
		glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess);

};