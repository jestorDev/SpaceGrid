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
#include <algorithm>
#include "VertexBuffer.hpp"
#include "Objeto3d.hpp"
#include "Turret.hpp"
#include "Proyectiles.hpp"
using namespace std;

#define numVAOs 20
#include "Nave.hpp"

struct Camera
{
	glm::vec3 pos;
	glm::vec3 lookingPoint;
} mainCam;

GLuint renderingProgram;
GLuint vao[numVAOs];
VertexBuffer cuadrado;

GLuint sueloTextura, goldTextura;

bool disparar;

GLuint mvLoc, projLoc,bgLoc,resLoc,timeLoc;
int Objeto3d::objectCount = 0;
bool primera;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
stack<glm::mat4> mvStack;
Objeto3d mycuadrado;
void bindGLTexture(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

bool tieneTorreta (vector<Turret> &torretas  , int i,int j){
	for (Turret &torreta : torretas)
	{
		if ( glm::length(torreta.getPos() - glm::vec3((float)i, 0, (float)j)) < 0.1)
			return true;
	}
	return false;
}
bool salioDelEscenario( Proyectil &proyectil){

	if (abs(proyectil.pos.x) > 13.0f)
		return true;
	if ( abs(proyectil.pos.z) > 40.0f)
		return true;
	if (proyectil.pos.y < -4.0f)
		return true;

	return false;
}


void setupVertices(void)
{
	const float sqareVertex[18] =
		{
			0.0f,
			0.0f,
			0.0f, //P1
			1.0f,
			0.0f,
			1.0f, //P2
			0.0f,
			0.0f,
			1.0f, //P3
			1.0f,
			0.0f,
			1.0f, //P2
			0.0f,
			0.0f,
			0.0f, //P1
			1.0f,
			0.0f,
			0.0f,
		};
	float squareTexCoor[12] =
		{
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f};

	glGenVertexArrays(numVAOs, vao);
	mycuadrado = Objeto3d(vao, VertexBuffer(sqareVertex, 18 * 4, 3, GL_FLOAT), VertexBuffer(squareTexCoor, 12 * 4, 2, GL_FLOAT), 18);
}

void init(GLFWwindow *window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
//	backgroundrenderingProgram = Utils::createShaderProgram("vertShader.glsl", "fondo.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	sueloTextura = Utils::loadTexture("../modelos-comprobados/suelo0.jpg");
	goldTextura = Utils::loadTexture("../modelos-comprobados/gold.jpg");
	mainCam.lookingPoint = glm::vec3(0.0f, 0, 0.0f);
	mainCam.pos = glm::vec3(0.0, 3.0, 5.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
void mostrar_fondo(Objeto3d *objeto, glm::mat4 vMatActua, GLuint vaos[numVAOs])
{
	const float h = -5;
	vMatActua *= glm::translate(glm::mat4(1.0f), glm::vec3(-200, h, -200));
	vMatActua *= glm::scale(glm::mat4(1.0f), glm::vec3(500, 1, 500));
	objeto->drawNoTexture(vaos, mvLoc, vMatActua);
}


void mostrar_trasladado(Objeto3d *objeto, glm::vec3 traslado, glm::mat4 vMatActua, GLuint texture, GLuint vaos[numVAOs] , bool escalado= true)
{
	const float h =  0;//0.1 * sin(glfwGetTime() + sqrt(traslado.x * traslado.x + traslado.y * traslado.y));
	vMatActua *= glm::translate(glm::mat4(1.0f), traslado - glm::vec3(0.5, h, 0.5));
	if (escalado)
		vMatActua *= glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.8, 0.8));
	objeto->draw(vaos, texture, mvLoc, vMatActua);
}

void display(GLFWwindow *window, double currentTime, Nave &nave, vector<Turret> &torretas, vector<Proyectil> &proyectiles)
{

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	bgLoc = glGetUniformLocation(renderingProgram, "background");
	resLoc = glGetUniformLocation(renderingProgram, "resolution");
	timeLoc = glGetUniformLocation(renderingProgram, "time");

	vMat = glm::lookAt(mainCam.pos,
					   mainCam.lookingPoint,
					   glm::vec3(0.0f, 1.0f, 0.0f));

	mMat = glm::translate(glm::mat4(1.0f), {0, 0, 0});
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform1i(bgLoc , 1);
	glUniform2f(resLoc , (float)width , (float)height);
	glUniform1f(timeLoc ,(float) currentTime);
	//	giros = {M_PI,0,0,0,M_PI,0,M_PI/2,0, 0 ,};
	mainCam.pos = nave.getPos() + glm::vec3(0.0f, 2.0f, -2.5f);
	mainCam.lookingPoint = nave.getPos();

	// Todos los render //////////////////////////////////////////

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(208.0, 239.0, 242.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	mostrar_fondo(&mycuadrado, vMat, vao);

	glUniform1i(bgLoc , 0);


	nave.render(currentTime, vMat, mvLoc, vao);

	for (auto &torreta : torretas)
	{
		torreta.render(currentTime, vMat, mvLoc, vao);
	}
	for (auto &proyectil : proyectiles)
	{
		proyectil.render(vMat, mvLoc, vao, currentTime);
		if (salioDelEscenario(proyectil)){
			proyectil.proximoEliminar = true; 
			std::cout << "asdasdasd\n"; 
		}
	}

	for (int i = -12; i < 12; i++)
	{
		for (int j = -40; j < 40; j++){
			if (j > 34)
			mostrar_trasladado(&mycuadrado, {(float)i, 0, (float)j}, vMat, goldTextura, vao , false	);
			else if (!tieneTorreta( torretas , i,j))
			mostrar_trasladado(&mycuadrado, {(float)i, 0, (float)j}, vMat, sueloTextura, vao);

		}
	}
	if (disparar)
	{
		//proyectiles.clear();
		for (auto &turret : torretas)
		{
			turret.lanzarProyectil(currentTime);
		}
		disparar = false;
	}

	// elimina los proyectiles marcados a eliminar
	proyectiles.erase(std::remove_if(
	    proyectiles.begin(), proyectiles.end(),
    	[](const Proyectil& p) { 
        	return p.proximoEliminar; // put your condition here
    	}), proyectiles.end());





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

	disparar = false;
	//array<Objeto3d, 3> modelosTorretas = {
	//	Objeto3d(vao, "../modelos-comprobados/turret3.obj"),
	//	Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
	//	Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
	//};

	//array<GLuint, 3> texturasTorretas = {
	//	Utils::loadTexture("../modelos-comprobados/turret3.jpg"),
	//	Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
	//	Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
	//};

	vector<Proyectil> proyectiles;
	FactoryProyectiles fproyectiles = FactoryProyectiles(vao, &proyectiles);
	FactoryTorretas fturets = FactoryTorretas(vao, &myNave, &fproyectiles);
	vector<Turret> torretas = {
		//	Turret(vao , {-4,0,8} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0] , &myNave ),
		//	Turret(vao , {-2,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0] ,  &myNave ),
		//	Turret(vao , {0,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),
		//	Turret(vao , {2,0,7} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),
		//	Turret(vao , {4,0,8} , Turret::tipo::Turret0 , &modelosTorretas[0],texturasTorretas[0]  ,  &myNave ),
		//	Turret(vao , {6,0,-2} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		//	Turret(vao , {5,0,-0} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		//	Turret(vao , {7,0, 2} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		//	Turret(vao , {6,0, 4} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		//	Turret(vao , {5,0, 6} , Turret::tipo::Turret1 , &modelosTorretas[1],texturasTorretas[1] ,   &myNave ),
		//	Turret(vao , {-2,0,2} , Turret::tipo::Turret2 , &modelosTorretas[2],texturasTorretas[2] ,  &myNave ),


		fturets.crearTorreta({-10, 0, -8 + 24}, Turret::Turret1),
		fturets.crearTorreta({ -8, 0, -8 + 24}, Turret::Turret1),
		fturets.crearTorreta({ -6, 0, -8 + 24}, Turret::Turret1),
		fturets.crearTorreta({ -4, 0, -6 + 24}, Turret::Turret1),
		fturets.crearTorreta({  0, 0, -4 + 24}, Turret::Turret1),
		fturets.crearTorreta({  4, 0, -6 + 24}, Turret::Turret1),
		fturets.crearTorreta({  6, 0, -8 + 24}, Turret::Turret1),
		fturets.crearTorreta({  8, 0, -8 + 24}, Turret::Turret1),
		fturets.crearTorreta({ 10, 0, -8 + 24}, Turret::Turret1),



		fturets.crearTorreta({-10, 0, -8 + 23}, Turret::Turret0),
		fturets.crearTorreta({ -8, 0, -6 + 23}, Turret::Turret0),
		fturets.crearTorreta({ -6, 0, -4 + 23}, Turret::Turret0),
		fturets.crearTorreta({ -4, 0, -2 + 23}, Turret::Turret0),
		fturets.crearTorreta({ -2, 0,  0 + 23}, Turret::Turret0),
		fturets.crearTorreta({  2, 0, -2 + 23}, Turret::Turret0),
		fturets.crearTorreta({  4, 0, -4 + 23}, Turret::Turret0),
		fturets.crearTorreta({  6, 0, -4 + 23}, Turret::Turret0),
		fturets.crearTorreta({  8, 0, -4 + 23}, Turret::Turret0),

		fturets.crearTorreta({10, 0,-8 + 23}, Turret::Turret0),
		fturets.crearTorreta({ 8, 0,-6 + 23}, Turret::Turret0),
		fturets.crearTorreta({ 6, 0,-4 + 23}, Turret::Turret0),
		fturets.crearTorreta({ 4, 0,-2 + 23}, Turret::Turret0),
		fturets.crearTorreta({ 2, 0, 0 + 23}, Turret::Turret0),
		fturets.crearTorreta({-2, 0,-2 + 23}, Turret::Turret0),
		fturets.crearTorreta({-4, 0,-4 + 23}, Turret::Turret0),
		fturets.crearTorreta({-6, 0,-4 + 23}, Turret::Turret0),
		fturets.crearTorreta({-8, 0,-4 + 23}, Turret::Turret0),





		fturets.crearTorreta({-9+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({-7+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({-5+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({-3+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({-1+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({ 1+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({ 3+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({ 5+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({ 7+1, 0, 10}, Turret::Turret2),
		fturets.crearTorreta({ 9+1, 0, 10}, Turret::Turret2),


		fturets.crearTorreta({-10, 0, -8}, Turret::Turret0),
		fturets.crearTorreta({ -8, 0, -6}, Turret::Turret0),
		fturets.crearTorreta({ -6, 0, -4}, Turret::Turret0),
		fturets.crearTorreta({ -4, 0, -2}, Turret::Turret0),
		fturets.crearTorreta({ -2, 0,  0}, Turret::Turret0),
		fturets.crearTorreta({  2, 0,  2}, Turret::Turret0),
		fturets.crearTorreta({  4, 0,  4}, Turret::Turret0),
		fturets.crearTorreta({  6, 0,  4}, Turret::Turret0),
		fturets.crearTorreta({  8, 0,  4}, Turret::Turret0),

		fturets.crearTorreta({10, 0,-8}, Turret::Turret0),
		fturets.crearTorreta({ 8, 0,-6}, Turret::Turret0),
		fturets.crearTorreta({ 6, 0,-4}, Turret::Turret0),
		fturets.crearTorreta({ 4, 0,-2}, Turret::Turret0),
		fturets.crearTorreta({ 2, 0, 0}, Turret::Turret0),
		fturets.crearTorreta({-2, 0, 2}, Turret::Turret0),
		fturets.crearTorreta({-4, 0, 4}, Turret::Turret0),
		fturets.crearTorreta({-6, 0, 4}, Turret::Turret0),
		fturets.crearTorreta({-8, 0, 4}, Turret::Turret0),




		fturets.crearTorreta({-9, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({-7, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({-5, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({-3, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({-1, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({ 1, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({ 3, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({ 5, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({ 7, 0, -10}, Turret::Turret2),
		fturets.crearTorreta({ 9, 0, -10}, Turret::Turret2),

		fturets.crearTorreta({-9, 0, -12}, Turret::Turret1),
		fturets.crearTorreta({-5, 0, -12}, Turret::Turret1),
		fturets.crearTorreta({-1, 0, -12}, Turret::Turret1),
		fturets.crearTorreta({ 3, 0, -12}, Turret::Turret1),
		fturets.crearTorreta({ 7, 0, -12}, Turret::Turret1),
		fturets.crearTorreta({ 9, 0, -12}, Turret::Turret1),



		fturets.crearTorreta({6, 0, -17}, Turret::Turret1),
		fturets.crearTorreta({-6, 0, -17}, Turret::Turret1),
		fturets.crearTorreta({ 9, 0, -21}, Turret::Turret1),
		fturets.crearTorreta({-9, 0, -21}, Turret::Turret1),
		fturets.crearTorreta({ 1, 0, -23}, Turret::Turret1),
		fturets.crearTorreta({-1, 0, -23}, Turret::Turret1),




		fturets.crearTorreta({-9+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({-7+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({-5+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({-3+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({-1+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({ 1+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({ 3+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({ 5+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({ 7+1, 0, -30}, Turret::Turret2),
		fturets.crearTorreta({ 9+1, 0, -30}, Turret::Turret2),


	};

	double currentTime = 0.0;
	primera = true;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		display(window, currentTime, myNave, torretas, proyectiles);
		glfwSwapBuffers(window);

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			myNave.setTipo(Nave::nave0);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			myNave.setTipo(Nave::nave1);
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			myNave.setTipo(Nave::nave2);
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			disparar = true;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			mainCam.lookingPoint.x += 0.1;
		if (myNave.ejecutandoAnimacion() == false)
		{
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				myNave.moverNave(glm::vec3(0.0f, 0.0f, 1.0f), currentTime);
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				myNave.moverNave(glm::vec3(0.0f, 0.0f, -1.0f), currentTime);
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				myNave.moverNave(glm::vec3(1.0f, 0.0f, 0.0f), currentTime);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				myNave.moverNave(glm::vec3(-1.0f, 0.0f, 0.0f), currentTime);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}