#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "LightSource.h"

class PointLight : public LightSource{
public:

    PointLight() : LightSource(){
        type = POINT;
        

        name = "pointLight" + to_string(nPoints);
        model = Model{"models/pointLight.obj"};
        shader = Shader{"lightSource.vs", "lightSource.fs"};
        diffuse = vec3{1.0f, 1.0f, 1.0f};
        specular = vec3{1.0f, 1.0f, 1.0f};
        ambient = vec3{0.0f, 0.0f, 0.0f};
        itensity = 1.0f;
        specItensity = 1.0f;

        constant = 1.0;
        linear = 0.5;      
        quadratic = 0.035;

        nPoints++;
        
    }
};


#endif
