#include "Helicopter.h"

Shader *shader;
glm::vec4 Copter_Ambient{ 0.8, 0.8, 0.8, 1.0 };
glm::vec4 Copter_Diffuse{ 0.8, 0.8, 0.5, 1.0 };
glm::vec4 Copter_Specular{ 0.9, 0.9, 0.8, 1.0 };
float Copter_Shininess = 10;

Helicopter::Helicopter()
{
	this->position = glm::vec3(200, 0, 0);
	
}

void Helicopter::render()
{
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_ambient"), 1, &Copter_Ambient[0]);
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_diffuse"), 1, &Copter_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_specular"), 1, &Copter_Specular[0]);
	glUniform1f(glGetUniformLocation(shader->handle(), "material_shininess"), Copter_Shininess);
	heliModel.drawElementsUsingVBO(shader);
}
void Helicopter::update()
{

}
void Helicopter::loadModel(Shader &myShader)
{
	
	shader = &myShader;
	cout << " loading heliModel " << endl;
	if (heliObjLoader.loadModel("TestModels/copter.obj", heliModel))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " heliModel loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		heliModel.calcCentrePoint();
		heliModel.centreOnZero();


		heliModel.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		heliModel.initDrawElements();
		heliModel.initVBO(&myShader);
		heliModel.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	cout << "LARGEST Exent = " << this->heliModel.theBBox.getLargestExtent() << endl;
	this->radious = this->heliModel.theBBox.getLargestExtent() / 4;
	
}

bool Helicopter::checkCollision(BoundingBox BB)
{
	return true;
}