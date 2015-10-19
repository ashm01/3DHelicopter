#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "shaders/Shader.h"



class Propeller
{
private:

	ThreeDModel propModel;
	OBJLoader propObjLoader;

public:

	Propeller();
	glm::mat4 ModelViewMatrix;
	double propRad;
	glm::vec3 position;
	void render(float propSpin);
	void update();
	void loadModel(Shader &myShader);
	glm::vec3 offset;

};