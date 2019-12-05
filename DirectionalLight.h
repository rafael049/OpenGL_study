#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "LightSource.h"

class DirectionalLight : public LightSource{
public:


    DirectionalLight() : LightSource(){
        type = DIRECTIONAL;

        castShadow = true;

        name = "directLight";
        model = Model{"models/pointLight.obj"};
        shader = Shader{"lightSource.vs", "lightSource.fs"};
        diffuse = vec3{1.0f, 1.0f, 1.0f};
        specular = vec3{1.0f, 1.0f, 1.0f};
        ambient = vec3{0.0f, 0.0f, 0.0f};
        itensity = 1.0f;
        specItensity = 1.0f;

        direction = vec3{1.0f, -1.0f, -1.0f};

    }

    void createShadowMap(){
        glGenFramebuffers(1, &depthFBO);

        glGenTextures(1, &shadowDepthMap);
        glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);




        
        


        
    }
};


#endif
