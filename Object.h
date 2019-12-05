#ifndef OBJECT_H
#define OBJECT_H

#include "glad.h"
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Model.h"

using namespace glm;

class Object{
public:

    // Name
    string name = "null";

    // Transform vectors
    vec3 pos;
    vec3 sca;
    vec4 rot;

    // Transform matrices
    mat4 localMat = mat4{1.0f};
    mat4 modelMat = mat4(1.0f);
    
    
    //Shader
    Shader shader{"shader1.vs", "shader1.fs"};

    // Model
    Model model{"models/cube.obj"};

    // Material
    vec3 lightColor{1.0f, 1.0f, 1.0f};
    vec3 color{1.0f, 0.5f, 0.31f};
    float shininess = 32;
    float specular = 0.5;
    bool reflect = false;
    float reflectLevel = 0.5;

    Object(){
        shader.setMat4f("local", value_ptr(localMat));
        shader.setMat4f("model", value_ptr(modelMat));

        shader.setFloat("material.shininess", shininess);
    }

    void setPosition(float x, float y, float z){
        modelMat[3] = vec4{0.0f, 0.0f, 0.0f, 1.0f};
        pos = vec3{x, y, z};
        modelMat = translate(modelMat, pos);
    }

    void setPosition(vec3 _pos){
        modelMat[3] = vec4{0.0f, 0.0f, 0.0f, 1.0f};
        pos = _pos;
        modelMat = translate(modelMat, pos );
    }

    void setScale(float s){
        localMat[0].x = 1.0f;
        localMat[1].y = 1.0f;
        localMat[2].z = 1.0f;
        sca = vec3{1.0f*s, 1.0f*s, 1.0f*s};
        localMat = scale(localMat, sca);
    }

    void rotateInAxis(float angle, vec3 axis){
        //model = rotate()
    }

    void setColor(vec3 _color){
        color = _color;
    }

    virtual void render(mat4 viewMat, mat4 projectionMat, unsigned int environmentMap = 0){

        shader.setMat4f("local", value_ptr(localMat));
        shader.setMat4f("model", value_ptr(modelMat));
        shader.setMat4f("view", value_ptr(viewMat));
        shader.setMat4f("projection", value_ptr(projectionMat));

        shader.setFloat("material.specular", specular);
        
        
        if(reflect){
            model.Draw(shader, environmentMap);
            shader.setBool("material.reflection", true);
            shader.setFloat("material.reflectLevel", reflectLevel);
        }
        else{
            model.Draw(shader);
            shader.setBool("material.reflection", false);
        }
    }

    void updateShaders(){
        /*shader.setVec3("objColor", color);
        shader.setVec3("lightColor", lightColor);*/
    }

};

#endif
