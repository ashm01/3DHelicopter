#include "Propeller.h"

Shader *propShader;
glm::vec4 Prop_Ambient{ 0.8, 0.8, 0.8, 1.0 };
glm::vec4 Prop_Diffuse{ 0.8, 0.8, 0.5, 1.0 };
glm::vec4 Prop_Specular{ 0.9, 0.9, 0.8, 1.0 };
float Prop_Shininess = 10;


Propeller::Propeller()
{

}

void Propeller::render(float propSpin)
{
	glUniform4fv(glGetUniformLocation(propShader->handle(), "material_ambient"), 1, &Prop_Ambient[0]);
	glUniform4fv(glGetUniformLocation(propShader->handle(), "material_diffuse"), 1, &Prop_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(propShader->handle(), "material_specular"), 1, &Prop_Specular[0]);
	glUniform1f(glGetUniformLocation(propShader->handle(), "material_shininess"), Prop_Shininess);
	ModelViewMatrix = glm::translate(ModelViewMatrix, offset);
	ModelViewMatrix = glm::rotate(ModelViewMatrix, propSpin, glm::vec3(0, 1, 0)); //tilts the cube around z
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.1, 0.1, 0.1));
	//prop.ModelViewMatrix = glm::translate(prop.ModelViewMatrix, glm::vec3(2, 0, 0));
	//Pass the uniform for the modelview matrix - in this case just "r"
	glUniformMatrix4fv(glGetUniformLocation(propShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	propModel.drawElementsUsingVBO(propShader);
}
void Propeller::update()
{

}
void Propeller::loadModel(Shader &myShader)
{

	propShader = &myShader;
	offset = glm::vec3(0, 3, 9);
	cout << " loading heliModel " << endl;
	if (propObjLoader.loadModel("TestModels/testProp.obj", propModel))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " heliModel loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		propModel.calcCentrePoint();
		propModel.centreOnZero();


		propModel.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		propModel.initDrawElements();
		propModel.initVBO(&myShader);
		propModel.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	this->propRad = this->propModel.theBBox.getLargestExtent() / 10;
}