#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "shaders/Shader.h"
#include "BoundBox.h"



class Helicopter
{
private:

	
	OBJLoader heliObjLoader;

public:
	ThreeDModel heliModel;
	double radious;
	BoundBox boundBox;
	Helicopter();
	glm::mat4 ModelViewMatrix;
	glm::vec3 position;
	void render();
	void update();
	void loadModel(Shader &myShader);
	bool checkCollision(BoundingBox BB);

};