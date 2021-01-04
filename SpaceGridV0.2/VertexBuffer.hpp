#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <GL/glew.h>

class VertexBuffer
{
private:
    GLuint gl_id;
    bool isIndexBuffer;
    GLint elementCount;
    GLenum type;
public:
    VertexBuffer(const void * data , unsigned int size,GLint elementCount, GLenum type, bool isIndexBuffer = false);
    VertexBuffer();
    ~VertexBuffer();
    void Delete();
    GLuint getId();
    void Bind(GLuint atribIdex);
    void Unbind();
};

#endif