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
    Turret::tipo modelo;
    glm::vec3 pos;
    float giroy;
    std::array<Objeto3d, 3> modelos;
    std::array<GLuint, 3> texturas;
    const std::array<glm::vec3, 3> escalamientos = {
        glm::vec3(0.05, 0.05, 0.05),
        glm::vec3(0.04, 0.04, 0.04),
        glm::vec3(1.0, 1.0, 1.0),
    };
    const std::array<double, 3> girpyIncial = {M_PI/2,0, 0 };
    const std::array<double, 3> frecuencia = {0.2, 0.30, 0.25};
    bool ejecutandoTraslacion;
    bool ejecutandoRotacion;
    AnimacionTraslacion miTraslacion;

public:
    Turret(GLuint *arrVaos, glm::vec3 posInicial);
    ~Turret();
    void setTipo(Turret::tipo nuevo) { modelo = nuevo; }
    bool ejecutandoAnimacion()
    {
        return ejecutandoTraslacion;
    }
    glm::vec3 getPos()
    {
        return pos;
    }

    void moverTurret(glm::vec3 desplazamiento, double tiempoInicio)
    {
        //giroz += (desplazamiento.x > 0)? 0.1;
        ejecutandoTraslacion = true;
        miTraslacion = {tiempoInicio, pos, desplazamiento, frecuencia[modelo]};
    }

    void actualizarAnimaciones(double time)
    {

        //traslacion
        if (ejecutandoTraslacion)
            pos = miTraslacion.nuevoPos(time, ejecutandoTraslacion);
        //
        if (ejecutandoTraslacion)
            pos = miTraslacion.nuevoPos(time, ejecutandoTraslacion);
        // idle rotacion
        //giroz = (M_PI / 10.0) * sin(time * 2);
        // idle rotacion
        pos.y = 1 + 0.3 * (sin(time * 3 + 0.2) + sin(time * 4));
    }

    void render(double time, glm::mat4 vMatActua, GLuint mvLoc, GLuint *arrVaos)
    {
        actualizarAnimaciones(time);
        vMatActua *= glm::translate(glm::mat4(1.0f), pos);
        vMatActua *= glm::rotate(glm::mat4(1.0f), giroy, glm::vec3(0.0f, 1.0f, 0.0f));
        vMatActua *= glm::scale(glm::mat4(1.0f), escalamientos[modelo]);
        modelos[modelo].draw(arrVaos, texturas[modelo], mvLoc, vMatActua);
    }
};

Turret::Turret(GLuint *arrVaos, glm::vec3 posInicial)
{
    ejecutandoTraslacion = false;
    pos = {0.0f, 1.0f, 0.0f};
    modelo = Turret1;
    giroy = girpyIncial[modelo];

    modelos = {
        Objeto3d(arrVaos, "../modelos-comprobados/turret2.obj"),
        Objeto3d(arrVaos, "../modelos-comprobados/turret0.obj"),
        Objeto3d(arrVaos, "../modelos-comprobados/turret1.obj"),
    };

    texturas = {
        Utils::loadTexture("../modelos-comprobados/turret2.jpg"),
        Utils::loadTexture("../modelos-comprobados/turret0.jpg"),
        Utils::loadTexture("../modelos-comprobados/turret1.jpg"),
    };

    std::cout << "Cargados modelos y texturas de las Turrets";
}

Turret::~Turret()
{
}

#endif // Turret_HH
