#ifndef VAO_H
#define VAO_H

#include "glad.h"

class VAO{
public:
    unsigned int ID;

    VAO(){
        glGenVertexArrays(1, &ID);
        glBindVertexArray(ID);
    }

    void bind(){
        glBindVertexArray(ID);
    }
};

#endif
