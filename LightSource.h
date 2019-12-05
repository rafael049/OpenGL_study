#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Object.h"

using namespace glm;

enum LIGHT_TYPE{
    POINT,
    DIRECTIONAL,
    SPOT
};

class LightSource : public Object{
public:
    
    vec3 direction;

    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float itensity;
    float ambientStr;
    float specItensity;

    float constant = 0.5;
    float linear = 0.5;
    float quadratic = 0.5;

    bool castShadow = false;


    LIGHT_TYPE type = POINT;

    // Lights number
    static unsigned int nPoints;
    static unsigned int nSpot;

    unsigned int shadowDepthMap = 0;
    unsigned int depthFBO = 0;
    const unsigned int SHADOW_SIZE = 1024;

    
    
    


    LightSource():
        Object(){    
            ambient *= ambientStr;          
    }

    LightSource(vec3 _pos, vec3 _diffuse, float _itensity, float _specItensity){

        
        setPosition(_pos);
        diffuse = _diffuse;
        itensity = _itensity;
        specItensity = _specItensity;
    }

    void setDiffuse(vec3 _diffuse){
        diffuse = _diffuse;
    }

    vec3 getDiffuse(){
        return diffuse;
    }

    virtual void createShadowMap(){}


};

unsigned int LightSource::nPoints = 0;
unsigned int LightSource::nSpot = 0;

#endif
