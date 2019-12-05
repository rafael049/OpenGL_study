#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "LightSource.h"

class SpotLight : public LightSource{
public:

    SpotLight() : LightSource(){
        type = SPOT;
        nSpot++;

       
    }
};


#endif
