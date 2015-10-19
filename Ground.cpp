#include "Ground.h"

Shader *groundShader;
glm::vec4 Ground_Ambient{ 0.8, 0.8, 0.8, 1.0 };
glm::vec4 Ground_Diffuse{ 0.8, 0.8, 0.5, 1.0 };
glm::vec4 Ground_Specular{ 0.9, 0.9, 0.8, 1.0 };
float Ground_Shininess = 10;

Ground::Ground()
{
	this->position = glm::vec3(0, -20, 0);
}

void Ground::render()
{
	glUniform4fv(glGetUniformLocation(groundShader->handle(), "material_ambient"), 1, &Ground_Ambient[0]);
	glUniform4fv(glGetUniformLocation(groundShader->handle(), "material_diffuse"), 1, &Ground_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(groundShader->handle(), "material_specular"), 1, &Ground_Specular[0]);
	glUniform1f(glGetUniformLocation(groundShader->handle(), "material_shininess"), Ground_Shininess);
	groundModel.drawElementsUsingVBO(groundShader);
	
}

void Ground::loadModel(Shader &myShader)
{

	groundShader = &myShader;
	//Loading ground
	cout << " loading groundModel " << endl;
	if (groundObjLoader.loadModel("TestModels/floor.obj", groundModel))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " groundModel loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		groundModel.calcCentrePoint();
		groundModel.centreOnZero();


		groundModel.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		groundModel.initDrawElements();
		groundModel.initVBO(&myShader);
		groundModel.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	
}