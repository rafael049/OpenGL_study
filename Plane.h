#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

class Plane : public Object{
public:

    Plane(){
        name = "plane";
        model = Model{"models/plane.obj"};

        specular = 0.5f;
    }
};

#endif
