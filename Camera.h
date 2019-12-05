#ifndef CAMERA_H
#define CAMERA_H

#include"glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

using namespace glm;

enum CamMoveDir{
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};

class Camera{
public:

    // Vectors
    vec3 pos;
    vec3 target;
    vec3 direction;
    vec3 right;
    vec3 up;
    vec3 worldUp;
    vec3 front;

    // Rotation
    float pitch = 0.0f;
    float yaw = -90.0f;

    float movSpeed = 3.0f;
    float mouseSensivity = 0.15f;

    // Transform Matrices
    mat4 view = mat4(1.0f);
    mat4 projection;

    // Projection Parameters
    float fov = 45.0f;
    float start = 0.1f;
    float end = 100.0f;

    Camera(){
        // Get Vectors
        pos = vec3(0.0f, 0.0f, 3.0f);
        target = vec3(0.0f, 0.0f, 0.0f);
        direction = normalize(pos - target);
        right = normalize(cross(vec3(0.0f, 1.0f, 0.0f), direction));
        up = cross(direction, right);
        worldUp = vec3(0.0f, 1.0f, 0.0f);
        front = vec3(0.0f, 0.0f, -1.0f);

        updateVectors();
        // Set projection
        projection = perspective(radians(fov), 800.0f/600.0f, start, end);
    }

    Camera(vec3 _pos, vec3 _direction, mat4 _projection){
        // Get Vectors
        pos = _pos;

        direction = _direction;
        right = normalize(cross(vec3(0.0f, 1.0f, 0.0f), direction));
        up = cross(direction, right);
        worldUp = vec3(0.0f, 1.0f, 0.0f);
        front = vec3(0.0f, 0.0f, -1.0f);

        updateVectors();
        // Set projection
        projection = _projection;
    }

    mat4 getViewMatrix(){
        return lookAt(pos, pos + front, up);
    }

    mat4 getProjMatrix(){
        return projection;
    }

    mat4 setTarget(vec3 _target){
        target = _target;
    }

    mat4 setPosition(vec3 position){
        pos = position;
    }

    void processKeyboard(CamMoveDir dir, float deltaTime){
        if (dir == FORWARD)
            pos += movSpeed * front * deltaTime;
        if (dir == BACKWARD)
            pos -= movSpeed * front * deltaTime;
        if (dir == LEFT)
            pos -= normalize(cross(front, up)) * movSpeed * deltaTime;
        if (dir == RIGHT)
            pos += normalize(cross(front, up)) * movSpeed * deltaTime;
        if (dir == UP)
            pos += worldUp * movSpeed * deltaTime;
        if (dir == DOWN){
            pos -= worldUp * movSpeed * deltaTime;
        }
    }

    void processMouseMovement(float xoffset, float yoffset){
        xoffset *= mouseSensivity;
        yoffset *= mouseSensivity;

        yaw += xoffset;
        pitch -= yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        updateVectors();
    }

    void updateVectors(){
        vec3 _front;
        _front.x = cos(radians(yaw)) * cos(radians(pitch));
        _front.y = sin(radians(pitch));
        _front.z = sin(radians(yaw)) * cos(radians(pitch));
        
        front = normalize(_front);
        right = normalize(cross(front, vec3(0.0f, 1.0f, 0.0f))) * movSpeed;
        up = normalize(cross(right, front));

    }
};


#endif
