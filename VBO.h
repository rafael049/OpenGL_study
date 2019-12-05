#ifndef VBO_H
#define VBO_H

#include "glad.h"
#include <iostream>
class VBO{
public:
    unsigned int ID;

    VBO(float *data,unsigned int size,  GLenum rawType = GL_STATIC_DRAW){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VBO(){
        
    }

    void writeToBuffer(float *data, GLenum rawType = GL_STATIC_DRAW){
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, rawType);
    }

    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void setAttribpointer(unsigned int index, unsigned int size, GLenum type, GLsizei stepBytes,const void* start ){
        glVertexAttribPointer(index, size, type, GL_FALSE, stepBytes, start);
        glEnableVertexAttribArray(index);
    }
};

#endif
