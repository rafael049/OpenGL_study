#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include "Object.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Skybox.h"
using namespace std;

class SceneManager{
public:
    map <string, Object*> objects;
    map <string, LightSource*> lights;

    Camera *camera = nullptr;
    Camera *mainCamera = nullptr;
    Camera *dirCamera = nullptr;
    Skybox *skybox = nullptr;

    unsigned int nPoints = 0;
    unsigned int nDirectional = 0;
    unsigned int nSpot = 0;

    SceneManager(Camera &cam){
        mainCamera = &cam;
        camera = mainCamera;
        dirCamera = new Camera{vec3{0.0f, 0.0f, 0.0f}, vec3{0.0f, -1.0f, 0.0f}, ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f)};
        skybox = new Skybox;

    } 

    void addObject(Object &obj){
        objects.insert(pair<string, Object*>{obj.name, &obj});
    }

    void addLight(LightSource &light){
        lights.insert(pair<string, LightSource*>{light.name, &light});

        if(light.type == POINT)
            nPoints++;
        else if(light.type == DIRECTIONAL)
            nDirectional++;
        else if(light.type == SPOT)
            nSpot++;
    }

    void setCamera(Camera &cam){
        camera = &cam;
        
    }

    void update(){

        /*// Render to depth map
        for(auto const &lightPair : lights){

            LightSource *light = lightPair.second;

            if(light->castShadow){
                glViewport(0, 0, light->SHADOW_SIZE, light->SHADOW_SIZE);
                glBindFramebuffer(GL_FRAMEBUFFER, light->depthFBO);
                glClear(GL_DEPTH_BUFFER_BIT);

                camera = dirCamera;

                render();
  
            }
            
        }*/

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 800, 600);
        camera = mainCamera;
        render();
        
    }

    void render(){
        if(camera == nullptr || lights.size() == 0){
            cout << "SCENE_MANAGER ERROR: no lights or camera found" << endl;
        }

        
        //cout << lights.size() << endl;
        for(auto const &obj : objects){
            // Set light source position
            unsigned int i = 0;
            for(auto const &lightPair : lights){
                LightSource *light = lightPair.second;
                
                if(light->type == POINT){
                    
                    obj.second->shader.setVec3("pointLights["+ to_string(i) +"].position", light->pos);
                    obj.second->shader.setVec3("pointLights["+ to_string(i) +"].diffuse", light->diffuse);
                    obj.second->shader.setVec3("pointLights["+ to_string(i) +"].ambient", light->ambient);
                    obj.second->shader.setVec3("pointLights["+ to_string(i) +"].specular", light->specular);
                    obj.second->shader.setFloat("pointLights["+ to_string(i) +"].constant", light->constant); 
                    obj.second->shader.setFloat("pointLights["+ to_string(i) +"].linear", light->linear);
                    obj.second->shader.setFloat("pointLights["+ to_string(i) +"].quadratic", light->quadratic);
                    
                    obj.second->shader.setInt("nPointLights", nPoints);
                    
                    obj.second->shader.setVec3("viewPos", camera->pos);
                    //obj.second->shader.setVec3("material.ambient", vec3{1.0f, 0.5f, 0.31f});
                    light->render(camera->getViewMatrix(), camera->getProjMatrix());
                    
                    i++;

                    /* LIGTHS ESTAO SENDO REDENIZADAS MAIS VEZES QUE O NECESSARIO*/

                }
                else if(light->type == DIRECTIONAL){
                    obj.second->shader.setVec3("dirLight.position", light->pos);
                    obj.second->shader.setVec3("dirLight.direction", light->direction);
                    obj.second->shader.setVec3("dirLight.diffuse", light->diffuse);
                    obj.second->shader.setVec3("dirLight.ambient", light->ambient);
                    obj.second->shader.setVec3("dirLight.specular", light->specular);

                    
                    
                    obj.second->shader.setVec3("viewPos", camera->pos);
                    //obj.second->shader.setVec3("material.ambient", vec3{1.0f, 0.5f, 0.31f});
                    light->render(camera->getViewMatrix(), camera->getProjMatrix());
        
                }

                
            }
            /*obj.second->shader.setVec3("light.position", light->pos);
            obj.second->shader.setVec3("light.diffuse", light->diffuse);
            obj.second->shader.setVec3("light.ambient", light->diffuse*0.1f);
            obj.second->shader.setVec3("light.specular", vec3{1.0f, 1.0f, 1.0f});
            obj.second->shader.setVec3("viewPos", camera->pos);
            obj.second->shader.setVec3("material.ambient", vec3{1.0f, 0.5f, 0.31f});*/

            // set skybox
            /*if(obj.second->reflect){
                
                obj.second->shader.setInt("skybox", 16);
                obj.second->shader.setBool("material.reflection", true);
                obj.second->render(camera->getViewMatrix(), camera->getProjMatrix(), skybox->texture);
            }
            else
            {
                obj.second->shader.setInt("skybox", 16);
                obj.second->shader.setBool("material.reflection", false);
                obj.second->render(camera->getViewMatrix(), camera->getProjMatrix());
            }**/
            
            
            // draw object
            
            obj.second->render(camera->getViewMatrix(), camera->getProjMatrix(), skybox->texture);   
        }
        // Draw Skybox
        skybox->render(camera->getViewMatrix(), camera->getProjMatrix());
    }

    void depthRender(){
        for(auto const &obj : objects){
            obj.second->render(camera->getViewMatrix(), camera->getProjMatrix(), 0);
        }
    }

    
};

#endif
