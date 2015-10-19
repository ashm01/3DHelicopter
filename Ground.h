#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "shaders/Shader.h"



class Ground
{
private:

	ThreeDModel groundModel;
	OBJLoader groundObjLoader;

public:

	Ground();
	glm::mat4 ModelViewMatrix;
	glm::vec3 position;
	void render();
	void loadModel(Shader &myShader);

};