#if !defined(NAVE_HH)
#define NAVE_HH
#include <array>
#include "Objeto3d.hpp"
#include <glm/glm.hpp>
#include "Animacion.hpp"
#include "Objeto3d.hpp"
#include "Utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>

class Nave
{
public:
    enum tipo
    {
        nave0 = 0,
        nave1 = 1,
        nave2 = 2,
    };

private:
    Nave::tipo modelo;
    glm::vec3 pos;
    float giroz;
    float girozBase;
    std::array<Objeto3d, 3> modelos;
    std::array<GLuint, 3> texturas;

    const std::array< glm::vec3 , 3> escalamientos= {
        glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.02, 0.02, 0.02),
		glm::vec3(0.3, 0.3, 0.3),
        };   

    const std::array<double ,3> tMovimiento= { 0.2 , 0.30, 0.25};
    //tiempo de movimiento para cada modelo en segundos
    bool ejecutandoTraslacion;
    bool ejecutandoRotacion;
    AnimacionTraslacion miTraslacion;

public:
    Nave(GLuint *arrVaos);
    ~Nave();
    void setTipo(Nave::tipo nuevo){modelo = nuevo;}
    bool ejecutandoAnimacion (){
        return ejecutandoTraslacion;
    }
    glm::vec3 getPos(){
        return pos;
    }

    void moverNave(glm::vec3 desplazamiento, double tiempoInicio)
    {
        //giroz += (desplazamiento.x > 0)? 0.1;  
        ejecutandoTraslacion = true;
        miTraslacion = {tiempoInicio, pos, desplazamiento, tMovimiento[modelo]};
    }

    void actualizarAnimaciones(double time){
        
        //traslacion
        if (ejecutandoTraslacion)
            pos = miTraslacion.nuevoPos(time, ejecutandoTraslacion);
        //
        if (ejecutandoTraslacion)
            pos = miTraslacion.nuevoPos(time, ejecutandoTraslacion);
        // idle rotacion
        giroz = (M_PI/10.0)* 0.5*(sin(time*2)+sin(time*2+2));
        // idle rotacion
        pos.y = 1 + 0.1*(sin(time*3 + 0.2)+sin(time*4));
    }

    void render(double time , glm::mat4 vMatActua , GLuint mvLoc  , GLuint *arrVaos )
    {
        actualizarAnimaciones(time);
        //                                          parcheado ojala no de problemas ||||||
        vMatActua *= glm::translate(glm::mat4(1.0f), pos + glm::vec3( 0.0f , 0.0f , -1.0f));
	    vMatActua *= glm::rotate(glm::mat4(1.0f),giroz,glm::vec3(0.0f,0.0f,1.0f));
	    vMatActua *= glm::scale(glm::mat4(1.0f), escalamientos[modelo]);
	    modelos[modelo].draw(arrVaos, texturas[modelo], mvLoc, vMatActua);

    }
};

Nave::Nave(GLuint *arrVaos)
{
    ejecutandoTraslacion = false;
    pos = {0.0f, 0.0f, 0.0f};
    
    modelo = nave1;
    giroz = 0;

    modelos = {
        Objeto3d(arrVaos, "../modelos-comprobados/nave0.obj"),
        Objeto3d(arrVaos, "../modelos-comprobados/nave1.obj"),
        Objeto3d(arrVaos, "../modelos-comprobados/nave2.obj"),
    };

    texturas = {
        Utils::loadTexture("../modelos-comprobados/nave0.jpg"),
	 Utils::loadTexture("../modelos-comprobados/nave1.jpg"),
	 Utils::loadTexture("../modelos-comprobados/nave2.jpg"),
    };    
    
    std::cout<< "Cargados modelos y texturas de las naves" ;

}

Nave::~Nave()
{
}

#endif // NAVE_HH
