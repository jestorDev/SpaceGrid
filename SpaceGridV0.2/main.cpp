#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "ImportedModel.h"
#include "Utils.h"
#include <stack>
#include <vector>
#include "VertexBuffer.hpp"
#include "Objeto3d.hpp"
#include "Turret.hpp"
//#include "Factorys.hpp"
using namespace std;

#define numVAOs 8
#include "Nave.hpp"

struct Camera{
	glm::vec3 pos;
	glm::vec3 lookingPoint;
}mainCam;

GLuint renderingProgram;
GLuint vao[numVAOs];
VertexBuffer cuadrado;

GLuint sueloTextura;

GLuint mvLoc, projLoc;
int Objeto3d::objectCount = 0;

int width, height;
float aspect;
glm::mat4 pMat,vMat,mMat,mvMat;
stack<glm::mat4> mvStack;
Objeto3d mycuadrado;
bool primera ;
void bindGLTexture(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }




void setupVertices(void)
{
	const float sqareVertex[18] =
		{
			0.0f, 0.0f,0.0f, //P1
			1.0f, 0.0f,1.0f, //P2
			0.0f, 0.0f,1.0f, //P3
			1.0f, 0.0f,1.0f, //P2
			0.0f, 0.0f,0.0f, //P1
			1.0f, 0.0f,0.0f, };
	float squareTexCoor[12] =
		{
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f};


	glGenVertexArrays(numVAOs, vao);
	mycuadrado = Objeto3d(vao , VertexBuffer(sqareVertex, 18 * 4, 3, GL_FLOAT), VertexBuffer(squareTexCoor, 12 * 4, 2, GL_FLOAT), 18);
}

void init(GLFWwindow *window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	sueloTextura = Utils::loadTexture("../modelos-comprobados/suelo0.jpg");
	mainCam.lookingPoint = glm::vec3(0.0f,0,0.0f);
 	mainCam.pos = glm::vec3(0.0, 3.0, 5.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}



void mostrar_trasladado(Objeto3d *objeto,  glm::vec3 traslado, glm::mat4 vMatActua, GLuint texture, GLuint vaos[numVAOs])
{
	const float h = 0.1 * sin(glfwGetTime() + sqrt(traslado.x*traslado.x +traslado.y*traslado.y) );
	vMatActua *= glm::translate(glm::mat4(1.0f), traslado - glm::vec3(0.5 , h,0.5));
	vMatActua *= glm::scale(glm::mat4(1.0f), glm::vec3(0.8,0.8,0.8));
	objeto->draw(vaos, texture, mvLoc, vMatActua);
}


void display(GLFWwindow *window, double currentTime, Nave &nave , vector<Turret> &torretas)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::lookAt(mainCam.pos,
					   mainCam.lookingPoint,
					   glm::vec3(0.0f, 1.0f, 0.0f));

	mMat = glm::translate(glm::mat4(1.0f), {0,0,0});
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));


//	giros = {M_PI,0,0,0,M_PI,0,M_PI/2,0, 0 ,};
	mainCam.pos = nave.getPos() + glm::vec3(0.0f ,2.0f ,-2.5f);
	mainCam.lookingPoint = nave.getPos();

	// Todos los render //////////////////////////////////////////
	nave.render(currentTime , vMat ,  mvLoc  ,vao );
	for (Turret &torreta : torretas)
	{
		torreta.render(currentTime , vMat , mvLoc , vao);
	}
	

	for (int i = -10; i < 10; i++){
		for (int j =-10; j < 10; j++)
			mostrar_trasladado(&mycuadrado , {(float)i,0 ,(float)j}  , vMat , sueloTextura , vao ); 
	}
}
void window_size_callback(GLFWwindow *win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void)
{
	primera = true;
	
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow *window = glfwCreateWindow(1500, 900, "Chapter6 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	Nave myNave = Nave(vao);

	array<Objeto3d,3> modelosTorretas= {
        Objeto3d(vao, "../modelos-comprobados/turret3.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
	};

	array <GLuint,3> texturasTorretas = {
		Utils::loadTexture("../modelos-comprobados/turret3.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
	};

	//array<Objeto3d,3> modelosProyectil= {
    //    Objeto3d(vao, "../modelos-comprobados/bomb.obj"),
    //    Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
    //    Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
	//};
//
	//array <GLuint,3> texturasProyectil = {
	//	Utils::loadTexture("../modelos-comprobados/bomb.jpg"),
	//	Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
	//	Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
	//};



	vector<Proyectil> proyectiles; 


	vector<Turret> torretas ={
		Turret(vao , {-4,0,8} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0] , &myNave ),
		Turret(vao , {-2,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0] ,  &myNave ),
		Turret(vao , {0,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),
		Turret(vao , {2,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),
		Turret(vao , {4,0,8} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),

		Turret(vao , {6,0,-2} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		Turret(vao , {5,0,-0} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		Turret(vao , {7,0, 2} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		Turret(vao , {6,0, 4} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		Turret(vao , {5,0, 6} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		Turret(vao , {-2,0,2} , Turret::tipo::Turret2 , &modelosTorretas[2],texturasTorretas[2] ,  &myNave ),
	};


	double currentTime = 0.0;
	
	while (!glfwWindowShouldClose(window))
	{
		currentTime =glfwGetTime();
		display(window,  currentTime , myNave , torretas);
		glfwSwapBuffers(window);

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			myNave.setTipo(Nave::nave0);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			myNave.setTipo(Nave::nave1);
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			myNave.setTipo(Nave::nave2);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			mainCam.lookingPoint.x += 0.1;
		if (myNave.ejecutandoAnimacion()== false)
		{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			myNave.moverNave( glm::vec3(0.0f ,0.0f ,1.0f) ,currentTime );
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			myNave.moverNave( glm::vec3(0.0f ,0.0f ,-1.0f) ,currentTime );
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			myNave.moverNave( glm::vec3(1.0f ,0.0f ,0.0f) ,currentTime );
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			myNave.moverNave( glm::vec3(-1.0f ,0.0f ,0.0f) ,currentTime );
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}