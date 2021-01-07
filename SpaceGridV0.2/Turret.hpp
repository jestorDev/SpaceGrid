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


class Turret
{
public:
    enum tipo
    {
        Turret0 = 0,
        Turret1 = 1,
        Turret2 = 2,
    };

private:
    Nave * nave ;
    Turret::tipo forma;
    glm::vec3 pos;
    float giroy;
    Objeto3d * modelo;
    GLuint textura;
    const std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.03, 0.03, 0.03),
        glm::vec3(0.04, 0.04, 0.04),
        glm::vec3(0.03, 0.03, 0.03),
    };
    const std::array<double, 3> girpyIncial = {M_PI/2,0, 0 };
    const std::array<double, 3> frecuencia = {2.0, 4.0, 8.0};
    bool ejecutandoRotacion;

public:
    Turret(GLuint *arrVaos, glm::vec3 posInicial, Turret::tipo model , Objeto3d * modelo , GLuint textura );
    ~Turret();

    void moverTurret(glm::vec3 desplazamiento, double tiempoInicio)
    {
        //giroz += (desplazamiento.x > 0)? 0.1;
        
        //ejecutandoTraslacion = true;
       // miTraslacion = {tiempoInicio, pos, desplazamiento, frecuencia[modelo]};
    }

    void actualizarAnimaciones(double time)
    {

        //traslacion
        // idle rotacion
        //giroz = (M_PI / 10.0) * sin(time * 2);
        // idle rotacion
        
    }

    void render(double time, glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos)
    {
        actualizarAnimaciones(time);
        vMatActua *= glm::translate(glm::mat4(1.0f), pos);
        vMatActua *= glm::rotate(glm::mat4(1.0f), giroy, glm::vec3(0.0f, 1.0f, 0.0f));
        vMatActua *= glm::scale(glm::mat4(1.0f), escalamientos[forma]);
        modelo->draw(arrVaos, textura, mvLoc, vMatActua);
    }
};

Turret::Turret(GLuint *arrVaos, glm::vec3 posInicial, Turret::tipo model , Objeto3d * modelo , GLuint textura )
{
    pos = posInicial;
    forma = model;
    giroy = girpyIncial[forma];
    this->modelo = modelo;
    this->textura =  textura;
    // = {
    //    Utils::loadTexture("../modelos-comprobados/turret2.jpg"),
    //    Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
    //    Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
    //};

    std::cout << "Cargados modelos y texturas de las Turrets";
}

Turret::~Turret()
{
}

#endif // Turret_HH
