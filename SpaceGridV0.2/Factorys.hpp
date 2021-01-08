#if !defined(FACTORYS__HH)
#define FACTORYS__HH


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "Utils.h"
#include <stack>
#include <array>
#include <vector>
#include "VertexBuffer.hpp"
#include "Objeto3d.hpp"
#include "Turret.hpp"

struct factoryProyectiles{
    GLuint *  vaoArr;
	std::array<Objeto3d,3> modelosProyectil;
	std::array<GLuint,3> texturasProyectil;

	factoryProyectiles( GLuint * vao){
        vaoArr = vao;
		modelosProyectil= {
        Objeto3d(vao, "../modelos-comprobados/bomb.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
	};

		texturasProyectil = {
		Utils::loadTexture("../modelos-comprobados/bomb.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
	};
	}

	
};


struct factoryTorretas
{	
	Nave * punteroNave ;
    GLuint *  vaoArr;
	std::array<Objeto3d,3> modelosTorretas;
	std::array <GLuint,3> texturasTorretas;
    const std::array<glm::vec3, 3> escalamientos;
	factoryTorretas(GLuint * vao, Nave * nave){
		punteroNave = nave;

	    const std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.3, 0.3, 0.3),
        glm::vec3(0.04, 0.04, 0.04),
        glm::vec3(0.03, 0.03, 0.03),
    };

    	modelosTorretas= {
        Objeto3d(vao, "../modelos-comprobados/turret3.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
        Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
	};


		texturasTorretas = {
		Utils::loadTexture("../modelos-comprobados/turret3.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
		Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
	};
	}
	Turret crearTorreta( Turret::tipo tipo , glm::vec3 pos ,glm::vec3 direcion  = {0.0f,0.0f,0.0f} ){
		return Turret(
			vaoArr ,
			pos , 
			tipo, 
			&modelosTorretas[tipo],
			texturasTorretas[tipo],
			&punteroNave
			);

	}


};


#endif // FACTORYS__HH
