//OPENGL 3.2 EXAMPLE

//NB: DEPRECATED COMMANDS WILL NOT FUNCTION.
//SEE OPENGL QUICK REFERENCE FOR DETAILS

#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library  
//#include <gl\glu.h>			// Header File For The GLu32 Library 
//-- OPENGL FILES INCLUDED IN GLEW see below

#include <math.h>     //sin and cos included in this header file.

#include "console.h"        //Used for displaying the console
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
                              //Shader.h has been created using common glsl structure

#include "MatrixRoutines.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

//MODEL LOADING
//#include "3DStruct\threeDModel.h"
//#include "Obj\OBJLoader.h"
#include "Helicopter.h"
#include "Propeller.h"
#include "World.h"
#include "Model.h"
#include "Ground.h"
#include "CollisionDetection.h"
float amount = 0;
float temp = 0.00002;


CollisionDetection collisionDetection;
Helicopter copter;
Propeller prop; 
World world;
Ground ground;
Model houseOne, houseTwo, houseThree, houseFour, houseFive, houseSix, treeOne, treeTwo, treeThree, treeFour, treeFive;
Model treeSix, treeSeven, treeEight, treeNine, treeTen;
Model leafOne, leafTwo, leafThree, leafFour;
//--- OpenGL ---
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------
#include "Cube.h"
unsigned int m_vaoID;		    // vertex array object
unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
GLuint ibo;                     //identifier for the triangle indices

const int numOfVerts = 8;
const int numOfTris = 12;
float verts[24];
float cols[24];
unsigned int tris[36];

bool keys[256];			// Array Used in the keyboard method.
int countD = 0;
int camNum = 1;
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;

Shader myShader, cubeShader;  ///shader object 
Cube cubeOne, worldCube;
ConsoleWindow console;

float Xpos = 2.5;
float Ypos = 2.5;
float dayNight = 0;
float spinY = 0.0;
float tiltX = 0.0;
float tiltZ = 0.0;
float tiltForce = 2;
float power = 0;
float forwardVel = 0;
float upVel = 0;
float propVel = 0;
float downVel = 0;
float propSpin = 0;
#include <iostream>
using namespace std;
// Timer 
double timerFrequencyRecip = 0.00001;	// Reciprocal of performance counter frequency
double prevDeltaT = 1.0;			// Previous time elapsed
__int64 prevTime = 0;				// Previous count
bool initialisedPrevDeltaT = false;
vector<Model> houses;
vector<Model> trees;
vector<Model> leafs;
//Light Properties
glm::vec4 Light_Ambient_And_Diffuse{ 0.8, 0.8, 0.6, 1.0 };
glm::vec4 Light_Specular{ 0.5, 0.5, 0.5, 1.0 };
glm::vec4 LightPos{ 0.0, 4000.0, 0.0, 1.0 };

int screenWidth=480, screenHeight=480;

//OPENGL FUNCTION PROTOTYPES
void init();				//called in winmain when the program starts.
void createGeometry();      //used to create Vertex Array Objects
void reshape(int width, int height);
void display();				//called in winmain to draw everything to the screen
void update(double deltaT);     //called to check key presses

/*************    START OF OPENGL FUNCTIONS   ****************/

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing

void init()
{

	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
					//glClear(GL_COLOR_BUFFER_BIT) in the display function//will clear the buffer to this colour.

	// Shaders
	if(!myShader.load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}							
	if (!cubeShader.load("BasicView", "glslfiles/cubeShader.vert", "glslfiles/cubeShader.frag"))
	{
		cout << "failed to load shader" << endl;
	}
	cubeOne.setDim(15);
	cubeOne.constructGeometry(&cubeShader);
	worldCube.setDim(1000);
	worldCube.constructGeometry(&cubeShader);

	glEnable(GL_TEXTURE_2D);

	
	copter.loadModel(myShader);
	prop.loadModel(myShader);
	world.loadModel(myShader);
	ground.loadModel(myShader);
	houseOne.loadModel(myShader, "TestModels/dododododhouse2.obj", glm::vec3(50, 44, 50), 
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	houseTwo.loadModel(myShader, "TestModels/pyramidhouse2.obj", glm::vec3(-1250, 44, 1000),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	houseThree.loadModel(myShader, "TestModels/pyramidhouse2.obj", glm::vec3(-500, 44, -500),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	houseFour.loadModel(myShader, "TestModels/pyramidhouse2.obj", glm::vec3(1200, 44, 1200),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	houseFive.loadModel(myShader, "TestModels/dododododhouse2.obj", glm::vec3(50, 44, -1150),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	houseSix.loadModel(myShader, "TestModels/dododododhouse2.obj", glm::vec3(-900, 44, -500),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 0);
	treeOne.loadModel(myShader, "TestModels/tree.obj", glm::vec3(250, 20, 250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeTwo.loadModel(myShader, "TestModels/tree.obj", glm::vec3(350, 20, 250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeThree.loadModel(myShader, "TestModels/tree.obj", glm::vec3(300, 20, 250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeFour.loadModel(myShader, "TestModels/tree.obj", glm::vec3(0, 20, 750),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeFive.loadModel(myShader, "TestModels/tree.obj", glm::vec3(200, 20, 180),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeOne.loadModel(myShader, "TestModels/tree.obj", glm::vec3(-900, 20, 580),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeTwo.loadModel(myShader, "TestModels/tree.obj", glm::vec3(1040, 20, 1050),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeThree.loadModel(myShader, "TestModels/tree.obj", glm::vec3(-1260, 20, -250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeFour.loadModel(myShader, "TestModels/tree.obj", glm::vec3(1270, 20, 250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	treeFive.loadModel(myShader, "TestModels/tree.obj", glm::vec3(-230, 20, 250),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	leafOne.loadModel(myShader, "TestModels/leaf.obj", glm::vec3(150, -10, 580),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	leafTwo.loadModel(myShader, "TestModels/leaf.obj", glm::vec3(170, -10, 530),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	leafThree.loadModel(myShader, "TestModels/leaf.obj", glm::vec3(150, -10, 550),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	leafFour.loadModel(myShader, "TestModels/leaf.obj", glm::vec3(150, -10, 510),
		glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, glm::vec4{ 0.8, 0.8, 0.8, 1.0 }, 50);
	
	houses.push_back(houseOne);
	houses.push_back(houseTwo);
	houses.push_back(houseThree);
	houses.push_back(houseFour);
	houses.push_back(houseFive);
	houses.push_back(houseSix);
	trees.push_back(treeOne);
	trees.push_back(treeTwo);
	trees.push_back(treeThree);
	trees.push_back(treeFour);
	trees.push_back(treeFive);
	trees.push_back(treeSix);
	trees.push_back(treeSeven);
	trees.push_back(treeEight);
	trees.push_back(treeNine);
	trees.push_back(treeTen);
	leafs.push_back(leafOne);
	leafs.push_back(leafTwo);
	leafs.push_back(leafThree);
	leafs.push_back(leafFour);
	glEnable(GL_DEPTH_TEST);

	
}

void createGeometry()
{
	// First simple object
	float dim = 15.0;
	verts[0] = -dim;   verts[ 1] = -dim;  verts[ 2] = -dim;
	verts[3] = -dim;   verts[ 4] =  dim;  verts[ 5] = -dim;
	verts[6] =  dim;   verts[ 7] =  dim;  verts[ 8] = -dim;
	verts[9] =  dim;   verts[10] = -dim;  verts[11] = -dim;

	verts[12] = -dim;   verts[13] = -dim;  verts[14] = dim;
	verts[15] = -dim;   verts[16] =  dim;  verts[17] = dim;
	verts[18] =  dim;   verts[19] =  dim;  verts[20] = dim;
	verts[21] =  dim;   verts[22] = -dim;  verts[23] = dim;

	cols[0] = 0.0;   cols[ 1] = 0.0;  cols[ 2] = 0.0;
	cols[3] = 0.0;   cols[ 4] = 1.0;  cols[ 5] = 0.0;
	cols[6] = 0.0;   cols[ 7] = 0.0;  cols[ 8] = 1.0;
	cols[9] = 1.0;   cols[10] = 1.0;  cols[11] = 1.0;

	cols[12] = 1.0;   cols[13] = 0.0;  cols[14] = 0.0;
	cols[15] = 0.0;   cols[16] = 1.0;  cols[17] = 0.0;
	cols[18] = 0.0;   cols[19] = 0.0;  cols[20] = 1.0;
	cols[21] = 1.0;   cols[22] = 1.0;  cols[23] = 0.0;
	
	tris[0]=0; tris[1]=1; tris[2]=2;
	tris[3]=0; tris[4]=2; tris[5]=3;
	tris[6]=4; tris[7]=6; tris[8]=5;
	tris[9]=4; tris[10]=7; tris[11]=6;
	tris[12]=1; tris[13]=5; tris[14]=6;
	tris[15]=1; tris[16]=6; tris[17]=2;
	tris[18]=0; tris[19]=7; tris[20]=4;
	tris[21]=0; tris[22]=3; tris[23]=7;
	tris[24]=0; tris[25]=5; tris[26]=1;
	tris[27]=0; tris[28]=4; tris[29]=5;
	tris[30]=3; tris[31]=2; tris[32]=7;
	tris[33]=2; tris[34]=6; tris[35]=7;
		
	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);
	
	glGenBuffers(2, m_vboID);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation= glGetAttribLocation(myShader.handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(vertexLocation);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), cols, GL_STATIC_DRAW);
	GLint colorLocation= glGetAttribLocation(myShader.handle(), "in_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(colorLocation);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; 
	screenHeight = height;

	glViewport(0,0,width,height);						// set Viewport dimensions

	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 6000.0f);
}

void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); 
	


	glm::mat4 ViewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 fpLookat;
	switch (camNum)
	{
		case 1://normal benhind copter rotates with copter
			cameraPos = glm::vec3(0, -8, 35);
			cameraPos = copter.position - glm::rotate(cameraPos, spinY, glm::vec3(0, 1, 0));
			ViewMatrix = glm::lookAt(cameraPos, copter.position,
				glm::vec3(0, 1, 0));
			break;
		case 2://birdseye
			cameraPos = glm::vec3(0,-60,1);
			cameraPos = copter.position - glm::rotate(cameraPos, spinY, glm::vec3(0, 1, 0));
			ViewMatrix = glm::lookAt(cameraPos, copter.position,
				glm::vec3(0, 1, 0));
			break;
		case 3://first person
			fpLookat = glm::vec3(0, -5, -60);
			
			fpLookat = copter.position - glm::rotate(fpLookat, spinY, glm::vec3(0, 1, 0));
			
			cameraPos = glm::vec3(0, 0, -25);
			//cameraPos = glm::rotate(cameraPos, -tiltZ, glm::vec3(1, 0, 0));
			//cameraPos = glm::rotate(cameraPos, -tiltX, glm::vec3(0, 0, 1));
			cameraPos = copter.position - glm::rotate(cameraPos, spinY, glm::vec3(0, 1, 0));
			ViewMatrix = glm::lookAt(cameraPos, fpLookat,
				glm::vec3(0, 1, 0));
			
			break;
		case 4://side view
			cameraPos = glm::vec3(50, 0, 1);
			cameraPos = copter.position - glm::rotate(cameraPos, spinY, glm::vec3(0, 1, 0));
			ViewMatrix = glm::lookAt(cameraPos, copter.position,
				glm::vec3(0, 1, 0));
			break;
		case 5://cinematic
			cameraPos = glm::vec3(100, 100, 100);
			ViewMatrix = glm::lookAt(cameraPos, copter.position,
				glm::vec3(0, 1, 0));
			break;
		default:
			cameraPos = glm::vec3(0, -8, 35);
			cameraPos = copter.position - glm::rotate(cameraPos, spinY, glm::vec3(0, 1, 0));
			ViewMatrix = glm::lookAt(cameraPos, copter.position,
				glm::vec3(0, 1, 0));
	}
		

	

	glUseProgram(cubeShader.handle());
	//All of our geometry will have the same projection matrix.
	//we only need to set it once, since we are using the same shader.
	GLuint matLocation = glGetUniformLocation(cubeShader.handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	cubeOne.ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 20, 0));

	glUniformMatrix4fv(glGetUniformLocation(cubeShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &cubeOne.ModelViewMatrix[0][0]);
	//cubeOne.render();
	worldCube.ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 0, 0));

	glUniformMatrix4fv(glGetUniformLocation(cubeShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &worldCube.ModelViewMatrix[0][0]);

	//worldCube.render();

	glUseProgram(0);
	glUseProgram(myShader.handle());  // use the shader


	

	glUniform1f(glGetUniformLocation(myShader.handle(), "displacement"), amount);
	glUniformMatrix4fv(glGetUniformLocation(myShader.handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	
	
	//viewMatrix = glm::lookAt(glm::vec3(0, 0, -100), glm::vec3(30, 0, 0),
		//glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(myShader.handle(), "ViewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	//Changing from daylight to night setting
	

	//Lighting Properties passed to the shader
	if (dayNight == 0)
	{
		Light_Ambient_And_Diffuse = glm::vec4{ 0.8, 0.8, 0.6, 1.0 };
		Light_Specular = glm::vec4{ 0.8, 0.8, 0.8, 1.0 };
	}
	else 
	{
		Light_Ambient_And_Diffuse = glm::vec4{ 0.2, 0.2, 0.2, 1.0 };
		Light_Specular = glm::vec4{ 0.2, 0.2, 0.2, 1.0 };
	}
	glUniform4fv(glGetUniformLocation(myShader.handle(), "LightPos"), 1, &LightPos[0]);
	glUniform4fv(glGetUniformLocation(myShader.handle(), "light_ambient"), 1, &Light_Ambient_And_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(myShader.handle(), "light_diffuse"), 1, &Light_Ambient_And_Diffuse[0]);
	glUniform4fv(glGetUniformLocation(myShader.handle(), "light_specular"), 1, &Light_Specular[0]);


	

	//DRAW THE MODEL
	copter.ModelViewMatrix = glm::translate(ViewMatrix, copter.position);
	copter.ModelViewMatrix = glm::rotate(copter.ModelViewMatrix, spinY, glm::vec3(0, 1, 0));	//rotate object about y axis
	copter.ModelViewMatrix = glm::rotate(copter.ModelViewMatrix, tiltX, glm::vec3(0, 0, 1)); //tilts the cube around x
	copter.ModelViewMatrix = glm::rotate(copter.ModelViewMatrix, tiltZ, glm::vec3(1, 0, 0)); //tilts the cube around z
	
	//Pass the uniform for the modelview matrix 
	glUniformMatrix4fv(glGetUniformLocation(myShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &copter.ModelViewMatrix[0][0]);
	
	glm::mat4 normalMatrix;
	glm::mat4 NormalMatrix = ModelViewMatrix * normalMatrix;
	glUniformMatrix3fv(glGetUniformLocation(myShader.handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	
	copter.render();
	//model.drawElementsUsingVBO(&myShader);
	
	//prop.ModelViewMatrix = glm::translate(prop.ModelViewMatrix, glm::vec3(0, 0, 0));
	prop.ModelViewMatrix = copter.ModelViewMatrix;

	

	prop.render(propSpin);
	//Drawing the world
	
	
	world.ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 1790, 0));
	glUniformMatrix4fv(glGetUniformLocation(myShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &world.ModelViewMatrix[0][0]);
	world.render();
	
	ground.ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, -55, 0));
	glUniformMatrix4fv(glGetUniformLocation(myShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ground.ModelViewMatrix[0][0]);
	ground.render();

	//for (int i = 0; i < houses.size(); i++)
	//{
	//	houses[i].render(ViewMatrix);
	//}
	//for (int i = 0; i < trees.size(); i++)
	//{
	//	trees[i].render(ViewMatrix);
	//}
	houseOne.render(ViewMatrix);
	houseTwo.render(ViewMatrix);
	houseThree.render(ViewMatrix);
	houseFour.render(ViewMatrix);
	houseFive.render(ViewMatrix);
	houseSix.render(ViewMatrix);
	treeOne.render(ViewMatrix);
	treeTwo.render(ViewMatrix);
	treeThree.render(ViewMatrix);
	treeFour.render(ViewMatrix);
	treeFive.render(ViewMatrix);
	treeSix.render(ViewMatrix);
	treeSeven.render(ViewMatrix);
	treeEight.render(ViewMatrix);
	treeNine.render(ViewMatrix);
	treeTen.render(ViewMatrix);
	leafOne.render(ViewMatrix);
	leafTwo.render(ViewMatrix);
	leafThree.render(ViewMatrix);
	leafFour.render(ViewMatrix);
	

	glUseProgram(0); //turn off the current shader
}


void update(double deltaT)
{
	glm::vec3 heliMotion;
	

	if (power == 1)
	{
		if (keys['A'] && keys['W'])
		{
			heliMotion.x += forwardVel*deltaT;
			tiltX -= tiltForce *deltaT;
			heliMotion.z += forwardVel*deltaT;
			tiltZ += tiltForce *deltaT;

		}
		else if (keys['D'] && keys['W'])
		{
			heliMotion.x -= forwardVel*deltaT;
			tiltX += tiltForce *deltaT;
			heliMotion.z += forwardVel*deltaT;
			tiltZ += tiltForce *deltaT;

		}
		else if (keys[VK_UP] && keys['W'])
		{
			heliMotion.y += upVel*deltaT;
			heliMotion.z += forwardVel*deltaT;
			tiltZ += tiltForce *deltaT;

		}
		else
		if (keys[VK_LEFT])
		{
			spinY += 5 * deltaT;

		}else
		if (keys[VK_RIGHT])
		{
			spinY -= 5 * deltaT;

		}else
		if (keys[VK_UP])
		{
			upVel += 0.05*deltaT;
			propVel += 0.2*deltaT;
			if (upVel > 6)
				heliMotion.y += upVel * deltaT;
			
		}
		else
		if (keys[VK_DOWN])
		{
			
			heliMotion.y -= 20 * deltaT;

		}
		else
		if (keys['W'])
		{
			
			if (upVel > 5)
			{
				heliMotion.z += forwardVel*deltaT;
				tiltZ += tiltForce *deltaT;
				forwardVel += 0.1*deltaT;
			}

		}
		else
		if (keys['S'])
		{
			heliMotion.z -= forwardVel*deltaT;
			tiltZ -= tiltForce *deltaT;


		}
		else
		if (keys['A'])
		{
			heliMotion.x += forwardVel*deltaT;
			tiltX -= tiltForce *deltaT;
		}
		else
		if (keys['D'])
		{
			heliMotion.x -= forwardVel*deltaT;
			tiltX += tiltForce *deltaT;
		}
		else
		{
			//MOVEMENT WHEN POWER ON BUT NO BUTTON PRESS
			forwardVel -= 0.05*deltaT;
		}
	}
	else
	{
		propVel -= 0.1*deltaT;
		heliMotion.y -= 10 * deltaT;
		forwardVel = 0;
		upVel = 0;
		
	}


	if (keys['1'])
	{
		camNum = 1;
	}
	if (keys['2'])
	{
		camNum = 2;
	}
	if (keys['3'])
	{
		camNum = 3;
	}
	if (keys['4'])
	{
		camNum = 4;
	}
	if (keys['5'])
	{
		camNum = 5;
	}

	if (keys['P'])
	{
		if (power == 0)
		{
			power = 1;
			propVel = 1;
		}
		else
			power = 0;
		keys['P'] = false;
	}
	
	if (keys['N'])
	{
		if (dayNight == 0)
		{
			dayNight = 1;
		}
		else
			dayNight = 0;
		keys['N'] = false;
		cout << dayNight << endl;
	}
	
	//Calculating the poition of the copter
	glm::mat4 heliMatrix(1);
	heliMatrix = glm::rotate(heliMatrix, spinY, glm::vec3(0, 1, 0));
	glm::vec3 worldMotion = glm::vec3(heliMatrix * glm::vec4(heliMotion, 0));
	copter.position += worldMotion;
	//CHECKING FOR COLLISIONS
	//cout << houseOne.boxDimensions.x << houseOne.boxDimensions.y << " " << houseOne.boxDimensions.z << endl;
	for (int i = 0; i < houses.size(); i++)
	{
		if (collisionDetection.sphereAABB(copter.position, copter.radious, houses[i].position, houses[i].boxDimensions) ||
			collisionDetection.sphereAABB(copter.position+prop.offset, prop.propRad, houses[i].position, houses[i].boxDimensions))
		{
			copter.position -= worldMotion;
			cout << "hello" << endl;
		}
	}
	for (int i = 0; i < trees.size(); i++)
	{
		if (collisionDetection.sphereCheck(copter.position, copter.radious, trees[i].position, trees[i].modelRadious) ||
			collisionDetection.sphereCheck(copter.position + prop.offset, prop.propRad, trees[i].position, trees[i].modelRadious))
		{
			copter.position -= worldMotion;
		}
	}
	
	if (collisionDetection.sphereCheck(copter.position, copter.radious, leafOne.position, leafOne.modelRadious * 6))
	{
		glm::vec3 direction(leafOne.position.x - copter.position.x,
			0, leafOne.position.z - copter.position.z);
		glm::vec3 dd(glm::normalize(direction));
		leafOne.position += dd;	
		leafOne.position.y += 2;			
	}
	else
	{
		leafOne.position.y -= 1;
		if (leafOne.position.y < 0) leafOne.position.y = 0;
	}
	if (collisionDetection.sphereCheck(copter.position, copter.radious, leafTwo.position, leafTwo.modelRadious * 6))
	{
		glm::vec3 direction(leafTwo.position.x - copter.position.x,
			0, leafTwo.position.z - copter.position.z);
		glm::vec3 dd(glm::normalize(direction));
		leafTwo.position += dd;
		leafTwo.position.y += 2;
	}
	else
	{
		leafTwo.position.y -= 1;
		if (leafTwo.position.y < 0) leafTwo.position.y = 0;
	}
	if (collisionDetection.sphereCheck(copter.position, copter.radious, leafThree.position, leafThree.modelRadious * 6))
	{
		glm::vec3 direction(leafThree.position.x - copter.position.x,
			0, leafThree.position.z - copter.position.z);
		glm::vec3 dd(glm::normalize(direction));
		leafThree.position += dd;
		leafThree.position.y += 2;
	}
	else
	{
		leafThree.position.y -= 1;
		if (leafThree.position.y < 0) leafThree.position.y = 0;
	}
	if (collisionDetection.sphereCheck(copter.position, copter.radious, leafFour.position, leafFour.modelRadious * 6))
	{
		glm::vec3 direction(leafFour.position.x - copter.position.x,
			0, leafFour.position.z - copter.position.z);
		glm::vec3 dd(glm::normalize(direction));
		leafFour.position += dd;
		leafFour.position.y += 2;
	}
	else
	{
		leafFour.position.y -= 1;
		if (leafFour.position.y < 0) leafFour.position.y = 0;
	}
		
	


	//setting limits to the tilt, spin, power
	if (spinY>360) spinY = 0;
	if (tiltX>20) tiltX = 20; //capping the tilt to 20 degrees
	else if (tiltX < -20) tiltX = -20;
	if (tiltZ>10) tiltZ = 10;
	else if (tiltZ < -10) tiltZ = -10;

	if (tiltX < 0) tiltX += 1*deltaT;
	if (tiltX > 0) tiltX -= 1*deltaT;
	if (tiltZ < 0) tiltZ += 1*deltaT;
	if (tiltZ > 0) tiltZ -= 1*deltaT;
	
	if (upVel > 30) upVel = 30;
	if (upVel < 0) upVel = 0;
	if (forwardVel > 30) forwardVel = 30;
	if (forwardVel < 0) forwardVel = 0;

	//Ensuring the copter cannot go out of the boundaries(80 and below in y axis)
	if (copter.position.y < 0)
		copter.position.y = 0;
	if (copter.position.y > 3550)
		copter.position.y = 3550;
	if (copter.position.x < -1750)
		copter.position.x = -1750;
	if (copter.position.x > 1750)
		copter.position.x = 1750;
	if (copter.position.z < -1750)
		copter.position.z = -1750;
	if (copter.position.z > 1750)
		copter.position.z = 1750;
	
	if (propVel > 30) propVel = 30;
	if (propVel < 0) propVel = 0;
	propSpin += propVel*deltaT;
	if (propSpin > 360) propSpin = 0;
	//printing out the copter z y x values using a if to stop from slowing
	//the simulation
	//if (countD == 100)
	//{
	//	cout << "copx = " << copter.position.x << "  copy = " << copter.position.y << "   copz = " << copter.position.z << endl;
	//	//cout << "tree bb test = " << tree.BB.centrePoint << endl;
	//	//copter.position, copter.radious, tree.position, tree.modelRadious
	//	/*cout << "copter rad  = " << copter.radious << endl;
	//	cout << "treex = " << tree.position.x << "  treey = " << tree.position.y << "   treez = " << tree.position.z << endl;
	//	cout << "tree rad  = " << tree.modelRadious << endl;
	//	cout << "Collision = " << cdAnswer << endl;*/
	//	
	//}
	countD++;
	if (countD > 1000)
		countD = 0;

}


/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	console.Open();
	
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;
			//initialise timer
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			__int64 currentTime = t.QuadPart;

			__int64 ticksElapsed = currentTime - prevTime;					// Ticks elapsed since the previous time step
			double deltaT = double(ticksElapsed) * timerFrequencyRecip;		// Convert to seconds

			if (!initialisedPrevDeltaT) {
				// This is the first time step
				// So we do not have the time elapsed during the previous time step, since there is no previous time step

				// Cheat, and use the current time step time for both
				prevDeltaT = deltaT;

				// We have initialised prevDeltaT; set initialisedPrevDeltaT to true so we don't do this again
				initialisedPrevDeltaT = true;
			}


			// Advance timer
			prevTime = currentTime;					// use the current time as the previous time in the next step
			prevDeltaT = deltaT;

			update(deltaT);

			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
		
	}
	
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << "OpenGL version: " << GLVersionString << endl;

	//We can check the version in OpenGL 
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << "OpenGL Version: " << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	
	init();

	reshape(width, height);     					// Set Up Our Perspective GL Screen
	
	
	return true;									// Success
}



