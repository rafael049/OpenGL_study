#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "glad.h"
#include "glm/glm.hpp"
//#include "VAO.h"
//#include "VBO.h"
//#include "Texture.h"
#include "Shader.h"

using namespace std;
using namespace glm;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};


class Mesh{
public:
    // --- Mesh Data ---
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    unsigned int VAO, VBO, EBO;

    Texture textureDiffuse;
    Texture textureSpecular;


    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
        
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();

    }

    void setupMesh(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex posisitons
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Draw(Shader shader, unsigned int environmentMap = 0){
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        
        for( unsigned int i = 0; i < /*textures.size()*/1; i++){
            
            glActiveTexture(GL_TEXTURE0 + i);

            string number;
            string name = textures[i].type;

            if( name == "texture_diffuse")
                number = to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = to_string(specularNr++);
            
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            
        }

        
        glBindVertexArray(VAO);
        shader.setInt("skybox", 16);
        if(environmentMap){
            
            glActiveTexture(GL_TEXTURE16);
            glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap);
        }

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);

    }
};

#endif
