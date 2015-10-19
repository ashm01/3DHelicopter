#include "Model.h"

Shader *ModelShader;

Model::Model()
{
	
}

void Model::render(glm::mat4 ViewMatrix)
{

	glUniform4fv(glGetUniformLocation(ModelShader->handle(), "material_ambient"), 1, &Model_Ambient[0]);
	glUniform4fv(glGetUniformLocation(ModelShader->handle(), "material_diffuse"), 1, &Model_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(ModelShader->handle(), "material_specular"), 1, &Model_Specular[0]);
	glUniform1f(glGetUniformLocation(ModelShader->handle(), "material_shininess"), Model_Shininess);
	this->ModelViewMatrix = glm::translate(ViewMatrix, position);
	glUniformMatrix4fv(glGetUniformLocation(ModelShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &this->ModelViewMatrix[0][0]);
	threeDModel.drawElementsUsingVBO(ModelShader);

}

void Model::loadModel(Shader &myShader, char *filename, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
{

	this->position = position;
	
	this->Model_Ambient = ambient;
	this->Model_Diffuse = diffuse;
	this->Model_Specular = specular;
	this->Model_Shininess = shininess;
	ModelShader = &myShader;
	//Loading Model
	cout << " loading threeDModel " << endl;
	if (ModelObjLoader.loadModel(filename, threeDModel))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " threeDModel loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		threeDModel.calcCentrePoint();
		threeDModel.centreOnZero();


		threeDModel.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		threeDModel.initDrawElements();
		threeDModel.initVBO(&myShader);
		threeDModel.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
	this->modelRadious = this->threeDModel.theBBox.getLargestExtent() / 2;
	this->boxDimensions.x = this->threeDModel.theBBox.boxWidthX;
	this->boxDimensions.y = this->threeDModel.theBBox.boxWidthY*0.80;
	this->boxDimensions.z = this->threeDModel.theBBox.boxWidthZ;
}