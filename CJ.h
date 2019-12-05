#ifndef CJ_H
#define CJ_H

#include "Object.h"

class Cj : public Object{
public:
    Cj(){
        name = "cj";
        model = Model{"models/cj.obj"};
        setScale(0.025f);
        
        reflect = false;
        specular = 0.0f;
    }

};

#endif
