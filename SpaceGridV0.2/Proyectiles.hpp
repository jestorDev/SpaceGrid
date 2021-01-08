#if !defined(FACTORYS__HH)
#define FACTORYS__HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include "Utils.h"
#include <stack>
#include <array>
#include <vector>
#include "VertexBuffer.hpp"
#include "Objeto3d.hpp"
//#include "Turret.hpp"

struct Proyectil {
    glm::vec3  pos;
    Objeto3d * modelo;
    GLuint textura;
    glm::vec3  direccion;
    glm::vec3 escalado;


    void update (){ 
        //pos += direccion * 0.05f;
    } 

    void render (glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos){
        update();
        vMatActua *= glm::translate(glm::mat4(1.0f), pos + glm::vec3(-1.0 ,0.0,0.0));
        vMatActua *= glm::scale(glm::mat4(1.0f), escalado);
        modelo->draw(arrVaos, textura, mvLoc, vMatActua);
        return;
    }
    
};


class FactoryProyectiles
{
private:
    GLuint *vaoArr;
    std::vector<Proyectil> * proyectiles;
    std::array<Objeto3d, 3> modelosProyectil;
    std::array<GLuint, 3> texturasProyectil;
    std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.18, 0.18, 0.18),
        glm::vec3(0.1, 0.1, 0.1),
        glm::vec3(10.0, 10.0, 10.0),
    };

public:
    FactoryProyectiles(GLuint *vao , std::vector<Proyectil> * proyectiles)
    {
        this->proyectiles = proyectiles;
        vaoArr = vao;
        modelosProyectil = {
            Objeto3d(vao, "../modelos-comprobados/pro0.obj"),
            Objeto3d(vao, "../modelos-comprobados/pro2.obj"),
            Objeto3d(vao, "../modelos-comprobados/pro1.obj"),
        };

        texturasProyectil = {
            Utils::loadTexture("../modelos-comprobados/pro0.jpg"),
            Utils::loadTexture("../modelos-comprobados/pro2.jpg"),
            Utils::loadTexture("../modelos-comprobados/pro1.jpg"),
        };
    }

    void crearProyectil(
        unsigned tipo,
        glm::vec3 pos,
        glm::vec3 direccion)
    {
        proyectiles->push_back(
        {
            pos,
            &modelosProyectil[tipo],
            texturasProyectil[tipo],
            direccion,
            escalamientos[tipo]
        }
        );
    }
};


#endif // FACTORYS__HH
