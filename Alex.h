#ifndef ALEX_H
#define ALEX_H

#include "Object.h"

class Alex : public Object{
public:
    Alex(){
        name = "Alex";
        model = Model("models/alex.obj");
        shader.setFloat("material.shininess", 32);
        
        reflect = false;
        reflectLevel = 0.2f;
        specular = 1.0f;
    }

};

#endif
