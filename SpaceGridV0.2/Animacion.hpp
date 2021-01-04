#if !defined(ANIMACION_HH)
#define ANIMACION_HH
#include <glm/glm.hpp>
glm::vec3 roundv (glm::vec3 v ){
	return glm::vec3 ( round(v.x) , round(v.y),round(v.z));
}

struct AnimacionTraslacion
{
    double tiempoInicio;
    glm::vec3 posInicio;
	glm::vec3 desplazamiento;
    double duracion;
    bool redondear;


    float funcionInterpolacion (double x){return x;}

    glm::vec3 nuevoPos (double tiempo, bool &bloqueo ){
    const double porcentaje = (tiempo/duracion)-(tiempoInicio/duracion);
	if(porcentaje >= 1.0){
		bloqueo = false;
        if (redondear)
    		return  roundv(posInicio +desplazamiento);	
        return  posInicio +desplazamiento;	
	}
	return posInicio +  (desplazamiento * funcionInterpolacion(porcentaje));
}

};

struct AnimacionRotacion
{
    double tiempoInicio;
    float angzInicio;
    float diferencia;
	double duracion;   
};


#endif // ANIMACION_HH


