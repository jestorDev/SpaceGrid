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
#include "VertexBuffer.hpp"
#include "Objeto3d.hpp"
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
	//myObjetos[4] = Objeto3d(vao, "../modelos-comprobados/naveo.obj");
	//myObjetos[5] = Objeto3d(vao, "../modelos-comprobados/turret2.obj");
	//myObjetos[6] = Objeto3d(vao, "../modelos-comprobados/turret0.obj");
	//myObjetos[7] = Objeto3d(vao, "../modelos-comprobados/turret1.obj");
	//myObjetos[8] = Objeto3d(vao , VertexBuffer(sqareVertex, 18 * 4, 3, GL_FLOAT), VertexBuffer(squareTexCoor, 12 * 4, 2, GL_FLOAT), 18);
	mycuadrado = Objeto3d(vao , VertexBuffer(sqareVertex, 18 * 4, 3, GL_FLOAT), VertexBuffer(squareTexCoor, 12 * 4, 2, GL_FLOAT), 18);
}

void init(GLFWwindow *window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

//	texturas[5] = Utils::loadTexture("../modelos-comprobados/turret2.jpg");
//	texturas[6] = Utils::loadTexture("../modelos-comprobados/turret0.jpg");
//	texturas[7] = Utils::loadTexture("../modelos-comprobados/turret1.jpg");
	sueloTextura = Utils::loadTexture("../modelos-comprobados/suelo0.jpg");
	mainCam.lookingPoint = glm::vec3(0.0f,0,0.0f);
 	mainCam.pos = glm::vec3(0.0, 3.0, 5.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
//enum class ObjetoMostrar
//{
//	naveo = 4,
//	turret2 = 5,
//	turret0 = 6,
//	turret1 = 7,
//};


void mostrar_trasladado(Objeto3d *objeto,  glm::vec3 traslado, glm::mat4 vMatActua, GLuint texture, GLuint vaos[numVAOs])
{
	const float h = 0.1 * sin(glfwGetTime() + sqrt(traslado.x*traslado.x +traslado.y*traslado.y) );
	vMatActua *= glm::translate(glm::mat4(1.0f), traslado - glm::vec3(0.5 , h,0.5));
	vMatActua *= glm::scale(glm::mat4(1.0f), glm::vec3(0.8,0.8,0.8));
	objeto->draw(vaos, texture, mvLoc, vMatActua);
}


void display(GLFWwindow *window, double currentTime, Nave &nave)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//const float radius = 2.0f;
	//mainCam.pos= glm::vec3(sin(-currentTime) * radius , -mainCam.pos.y , cos(-currentTime) * radius );
	vMat = glm::lookAt(mainCam.pos,
					   mainCam.lookingPoint,
					   glm::vec3(0.0f, 1.0f, 0.0f));

//	myNave.pos.y=1.0 + 0.05f*(sin(currentTime*3) + sin(currentTime*4)); 
//	if (myNave.ejecutandoAnimacion)
//		moverNavePaso((currentTime / myNave.duracion ) - (myNave.inicioAnimacion/ myNave.duracion));
//	myNave.giroz =  (M_PI/10.0)*sin(currentTime*2);
//
	mMat = glm::translate(glm::mat4(1.0f), {0,0,0});
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

//	escalamientos = {
//		glm::vec3(1.0, 1.0, 1.0),
//		glm::vec3(0.08, 0.08, 0.08),
//		glm::vec3(0.05, 0.05, 0.05),
//
//		glm::vec3(1.0, 1.0, 1.0),
//		glm::vec3(0.1, 0.1, 0.1),
//		glm::vec3(0.03, 0.03, 0.03),
//
//		glm::vec3(0.05, 0.05, 0.05),
//		glm::vec3(0.04, 0.04, 0.04),
//		glm::vec3(1.0, 1.0, 1.0),
//	};

//	giros = {M_PI,0,0,0,M_PI,0,M_PI/2,0, 0 ,};
	nave.render(currentTime , vMat ,  mvLoc  ,vao );
	for (int i = -10; i < 10; i++){
		for (int j =-10; j < 10; j++)
			mostrar_trasladado(&mycuadrado , {(float)i,0 ,(float)j}  , vMat , sueloTextura , vao ); 
	}
	mainCam.pos = nave.getPos() + glm::vec3(0 ,2 ,-2);
	mainCam.lookingPoint = nave.getPos();
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
	GLFWwindow *window = glfwCreateWindow(1300, 900, "Chapter6 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	Nave myNave = Nave(vao);

	double currentTime = 0.0;
	
	while (!glfwWindowShouldClose(window))
	{
		currentTime =glfwGetTime();
		display(window,  currentTime , myNave);
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