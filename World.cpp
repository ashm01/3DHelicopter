#include "World.h"

Shader *worldShader;
glm::vec4 World_Ambient{ 0.8, 0.8, 0.8, 1.0 };
glm::vec4 World_Diffuse{ 0.8, 0.8, 0.5, 1.0 };
glm::vec4 World_Specular{ 0.9, 0.9, 0.8, 1.0 };
float World_Shininess = 10;

World::World()
{

}

void World::render()
{
	
	glUniform4fv(glGetUniformLocation(worldShader->handle(), "material_ambient"), 1, &World_Ambient[0]);
	glUniform4fv(glGetUniformLocation(worldShader->handle(), "material_diffuse"), 1, &World_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(worldShader->handle(), "material_specular"), 1, &World_Specular[0]);
	glUniform1f(glGetUniformLocation(worldShader->handle(), "material_shininess"), World_Shininess);
	skyModel.drawElementsUsingVBO(worldShader);
}

void World::loadModel(Shader &myShader)
{

	worldShader = &myShader;
	//Loading Sky
	cout << " loading skyModel " << endl;
	if (worldObjLoader.loadModel("TestModels/skybox2.obj", skyModel))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " skyModel loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		skyModel.calcCentrePoint();
		skyModel.centreOnZero();


		skyModel.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		skyModel.initDrawElements();
		skyModel.initVBO(&myShader);
		skyModel.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
}