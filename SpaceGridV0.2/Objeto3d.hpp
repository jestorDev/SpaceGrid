#ifndef OBJETO_3D
#define OBJETO_3D
#include <GL/glew.h>
#include <array>
#include "VertexBuffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective



class Objeto3d
{
private:
    static int objectCount;
    int number_id;
    int totalVertices;
    std::array <VertexBuffer,3> modelVBOs;
public:
    Objeto3d();
    Objeto3d(GLuint *myVaos, const char *modelPath);
    Objeto3d(GLuint *arrVaos, VertexBuffer puntos  ,VertexBuffer textura , int total_vertices);
    ~Objeto3d();
    void draw (GLuint *arrVaos, GLuint texura , GLuint mvLoc , glm::mat4 vMat );
    void drawNoTexture (GLuint *arrVaos, GLuint mvLoc , glm::mat4 vMat );
};


#endif
