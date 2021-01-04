#include <GL/glew.h>
#include "VertexBuffer.hpp"
#include <iostream>

VertexBuffer::VertexBuffer(const void *data, unsigned int size ,GLint elementCount,GLenum type , bool isIndBuffer)
{
    //size in bytes points.size()*sizeof(points[0])
    this->isIndexBuffer = isIndBuffer;
    this->elementCount = elementCount;
    this->type = type;

    glGenBuffers(1, &gl_id);
    if (!isIndexBuffer)
        glBindBuffer(GL_ARRAY_BUFFER, gl_id);
    else
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id);

    if (!isIndexBuffer){
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    else
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    std::cout << "LLamado el constructor buffer datos, nuevo" << ((isIndexBuffer) ? "indexBuffer " : "vertexBuffer ")<< gl_id << std::endl;
}

VertexBuffer::VertexBuffer()
{
    gl_id = 0;
    std::cout << "LLamado el constructor defecto" << std::endl;
}

GLuint VertexBuffer::getId()
{
    return gl_id;
}

VertexBuffer::~VertexBuffer()
{
    //    std::cout << "Desruido el vertex buffer"<<std::endl;
    //    glDeleteBuffers(1, &gl_id);
}
void VertexBuffer::Bind(GLuint atribIdex)
{
    if (!isIndexBuffer){
        glBindBuffer(GL_ARRAY_BUFFER, gl_id);
        glVertexAttribPointer(atribIdex, elementCount, type, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(atribIdex);        
    }
    else
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
    std::cout << "Desruido el vertex buffer" << std::endl;
    glDeleteBuffers(1, &gl_id);
}
