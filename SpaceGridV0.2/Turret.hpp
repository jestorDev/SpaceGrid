#if !defined(Turret_HH)
#define Turret_HH

#include <array>
#include "Objeto3d.hpp"
#include <glm/glm.hpp>
#include "Animacion.hpp"
#include "Objeto3d.hpp"
#include "Utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include "Nave.hpp"
#include "Proyectiles.hpp"



class Turret
{
public:
    enum tipo
    {
        Turret0 = 0,// canon
        Turret1 = 1,// azul
        Turret2 = 2,// rojo fijo
    };

private:
    Nave * nave ;
    Turret::tipo forma;
    glm::vec3 pos;
    float giroybase;
    float giroy;
    Objeto3d * modeloTorreta;
    FactoryProyectiles * factory;
    GLuint textura;
    glm::vec3 escalamiento;
    bool ejecutandoRotacion;

public:
    Turret(GLuint *arrVaos, 
        glm::vec3 posInicial, 
        Turret::tipo model , 
        Objeto3d * modeloTorreta ,
        GLuint textura , 
        float giroybase,
        glm::vec3 escalamiento, 
        FactoryProyectiles * factory,
        Nave * nave);
    ~Turret();

    void apuntarANave(glm::vec3 desplazamiento, double tiempoInicio)
    {   

        if(forma == Turret0 || forma == Turret1){

            const float dx = nave->getPos().x - pos.x; 
            const float dz = nave->getPos().z - pos.z;
            float angulo ;//= atan( (pos.z *nave->getPos().x - pos.x *nave->getPos().z  )/ (pos.x *nave->getPos().x + pos.z *nave->getPos().z  ));
            const float absdz = abs(dz); 
            if (absdz < 0.00000001)
                angulo =  (dx > 0)? M_PI/2: -M_PI/2;
            else 
                angulo = atan(dx/dz) ; // <<<< andgulo de giro
            if(dz <  0 && absdz > 0.00000001)
                angulo =  ((dx > 0)? M_PI: -M_PI )+ angulo;
                
            giroy = giroybase + angulo;
        }
    }

    void actualizarAnimaciones(double time)
    {
        apuntarANave({0,0,0,} , time);
        
        
    }
    glm::vec3 getPos (){
        return pos;
    }
    void lanzarProyectil (double time){
        glm::vec3 direccion =  glm::rotate(glm::mat4(1.0f), giroy -giroybase, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0,0,1,1) ; // { sin(giroy) ,0 , cos(giroy) };
        std::cout << direccion.x<< ' ' <<direccion.y<< ' ' <<direccion.z <<"\n " ;
        factory->crearProyectil((unsigned)forma , glm::vec3(0,.5,0)+ pos + direccion * 0.5f, direccion,time);
    }


    void render(double time, glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos)
    {
        actualizarAnimaciones(time);
        vMatActua *= glm::translate(glm::mat4(1.0f), pos);
        vMatActua *= glm::rotate(glm::mat4(1.0f), giroy, glm::vec3(0.0f, 1.0f, 0.0f));
        vMatActua *= glm::scale(glm::mat4(1.0f), escalamiento);
        modeloTorreta->draw(arrVaos, textura, mvLoc, vMatActua);
    }
};

Turret::Turret(GLuint *arrVaos, 
        glm::vec3 posInicial, 
        Turret::tipo model , 
        Objeto3d * modeloTorreta ,
        GLuint textura , 
        float giroybase,
        glm::vec3 escalamiento, 
        FactoryProyectiles * factory,
        Nave * nave
        )
{
    pos = posInicial;
    
    forma = model;

    this->giroybase = giroybase ;
    this->factory = factory;
    giroy = giroybase;
    
    this->escalamiento = escalamiento;
    this->nave = nave;
    this->modeloTorreta = modeloTorreta;
    this->textura =  textura;
    std::cout << "Cargados modeloTorretas y texturas de las Turrets";
    //lanzarProyectil();
}

Turret::~Turret()
{
}




class FactoryTorretas
{
private:
    Nave *punteroNave;
    GLuint *vaoArr;
    FactoryProyectiles * p_factory;
    std::array<Objeto3d, 3> modelosTorretas;
    std::array<GLuint, 3> texturasTorretas;
    std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.3, 0.3, 0.3),
        glm::vec3(0.04, 0.04, 0.04),
        glm::vec3(0.03, 0.03, 0.03),
    };
    const std::array<double, 3> girpyIncial = {M_PI, M_PI / 2.0, 0};
    const std::array<double, 3> frecuencia = {2.0, 4.0, 8.0};

public:
    FactoryTorretas(GLuint *vao, Nave *nave , FactoryProyectiles * factory)
    {
        punteroNave = nave;
        //	    escalamientos = {
        //        glm::vec3(0.3, 0.3, 0.3),
        //        glm::vec3(0.04, 0.04, 0.04),
        //        glm::vec3(0.03, 0.03, 0.03),
        //    };
        //    girpyIncial = {M_PI,M_PI/2.0, 0 };
        p_factory = factory;
        modelosTorretas = {
            Objeto3d(vao, "../modelos-comprobados/turret3.obj"),
            Objeto3d(vao, "../modelos-comprobados/turret0.obj"),
            Objeto3d(vao, "../modelos-comprobados/turret1.obj"),
        };

        texturasTorretas = {
            Utils::loadTexture("../modelos-comprobados/turret3.jpg"),
            Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
            Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
        };

        std::cout << "Cargados modelos y texturas de torretas";
    }

    Turret crearTorreta(
        glm::vec3 pos,
        Turret::tipo tipo,
        glm::vec3 direcion = {0.0f, 0.0f, 0.0f})
    {

        std::cout << "Creado torreta tipo: " << (int)tipo << "\n";
        return Turret(
            vaoArr,
            pos,
            tipo,
            &modelosTorretas[tipo],
            texturasTorretas[tipo],
            girpyIncial[tipo],
            escalamientos[tipo],
            p_factory,
            punteroNave);
    }
};


#endif // Turret_HH
