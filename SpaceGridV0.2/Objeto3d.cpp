#include "Objeto3d.hpp"
#include <GL/glew.h>
#include "ImportedModel.h"
#include <array>
#include "VertexBuffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

Objeto3d::Objeto3d(){

}

Objeto3d::Objeto3d(GLuint *arrVaos, const char *modelPath)
{
    ModelImporter myModel(modelPath);
    number_id = objectCount;
    glBindVertexArray(arrVaos[objectCount]);
    objectCount++;

    std::vector<float> pvalues = myModel.getVertices();
    std::vector<float> tvalues = myModel.getTextureCoordinates();
    std::vector<float> nvalues = myModel.getNormals();

    modelVBOs[0] = VertexBuffer(&pvalues[0], pvalues.size() * 4, 3, GL_FLOAT);
    modelVBOs[1] = VertexBuffer(&tvalues[0], tvalues.size() * 4, 2, GL_FLOAT);
    modelVBOs[2] = VertexBuffer(&nvalues[0], nvalues.size() * 4, 3, GL_FLOAT);
    
    modelVBOs[0].Bind(0);
    modelVBOs[1].Bind(1);
    modelVBOs[2].Bind(2);
    totalVertices = myModel.getNumVertices();
}

Objeto3d::Objeto3d(GLuint *arrVaos, VertexBuffer puntos  ,VertexBuffer textura, int total_vertices)
{
    number_id = objectCount;
    glBindVertexArray(arrVaos[objectCount]);
    objectCount++;


    modelVBOs[0] = puntos;
    modelVBOs[1] = textura;
//    modelVBOs[2] = nullptr;
    
    modelVBOs[0].Bind(0);
    modelVBOs[1].Bind(1);
//    modelVBOs[2].Bind(2);
    totalVertices = total_vertices;
}

Objeto3d::~Objeto3d()
{
}

void Objeto3d::draw(GLuint *arrVaos, GLuint texura , GLuint mvLoc , glm::mat4 vMat )
{
    glBindVertexArray(arrVaos[number_id]);
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));

  	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texura);
    glDrawArrays(GL_TRIANGLES, 0, totalVertices);
}
void Objeto3d::drawNoTexture (GLuint *arrVaos, GLuint mvLoc , glm::mat4 vMat )
{
    glBindVertexArray(arrVaos[number_id]);
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glDrawArrays(GL_TRIANGLES, 0, totalVertices);
}
