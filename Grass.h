#ifndef GRASS_H
#define GRASS_H

#include "Object.h"

class Grass : public Object{
public:
    static unsigned int count;
    Grass(){
        name = "Grass" + to_string(count);
        model = Model("models/grass.obj");
        shader.setFloat("material.shininess", 32);
        count++;
    }

};

unsigned int Grass::count = 0;

#endif
