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

struct Proyectil {
    glm::vec3  pos;
    Objeto3d * modelo;
    GLuint textura;
    glm::vec3  direccion;
    glm::vec3 escalado;

    void update (){ 
        pos += direccion * 0.05f;
    } 

    void render (glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos){
        update();
        vMatActua *= glm::translate(glm::mat4(1.0f), pos + glm::vec3(-1.0 ,0.0,0.0));
        vMatActua *= glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
        modelo->draw(arrVaos, textura, mvLoc, vMatActua);
        return;
    }
    
};


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
    GLuint textura;
    glm::vec3 escalamiento;
        
    const std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.3, 0.3, 0.3),
        glm::vec3(0.04, 0.04, 0.04),
        glm::vec3(0.03, 0.03, 0.03),
    };
    const std::array<double, 3> girpyIncial = {M_PI,M_PI/2.0, 0 };
    const std::array<double, 3> frecuencia = {2.0, 4.0, 8.0};
    bool ejecutandoRotacion;

public:
    Turret(
        GLuint *arrVaos, 
        glm::vec3 posInicial, 
        Turret::tipo model , 
        Objeto3d * modeloTorreta ,
        GLuint textura , 
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


    void render(double time, glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos)
    {
        actualizarAnimaciones(time);
        vMatActua *= glm::translate(glm::mat4(1.0f), pos);
        vMatActua *= glm::rotate(glm::mat4(1.0f), giroy, glm::vec3(0.0f, 1.0f, 0.0f));
        vMatActua *= glm::scale(glm::mat4(1.0f), escalamientos[forma]);
        modeloTorreta->draw(arrVaos, textura, mvLoc, vMatActua);
    }
};

Turret::Turret(GLuint *arrVaos, 
        glm::vec3 posInicial, 
        Turret::tipo model , 
        Objeto3d * modeloTorreta ,
        GLuint textura , 
        Nave * nave)
{
    pos = posInicial;
    forma = model;
    giroybase = girpyIncial[forma];
    giroy = giroybase;
    this->nave = nave;
    this->modeloTorreta = modeloTorreta;
    this->textura =  textura;
    std::cout << "Cargados modeloTorretas y texturas de las Turrets";
}

Turret::~Turret()
{
}

#endif // Turret_HH
