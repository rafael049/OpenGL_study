#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "glad.h"
#include "glm/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Mesh.h"

using namespace std;
using namespace glm;

unsigned int textureFromFile(const char *path, const string &directory, string typeName);

vector<Texture> textures_loaded;

class Model{
public:
    // Model data
    
    vector<Mesh> meshes;
    string directory;
    

    Model(const char *path){
        loadModel(path);
    }

    void loadModel(string path){
        Assimp::Importer import;

        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode *node, const aiScene *scene){

        for(unsigned int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene){
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices;i++){
            Vertex vertex;

            // process vertex posisitions
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // process normals
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            // process TextureCoords
            if(mesh->mTextureCoords[0]){
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else{
                vertex.texCoords = vec2{0.0f, 0.0f};
            }

            vertices.push_back(vertex);
        }
        // process indices

        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        }

        // process material
        if(mesh->mMaterialIndex >= 0){
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        }
        else{
            cout << "material index = 0" << endl;
        }

        return Mesh{vertices, indices, textures};
    }

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName){
        vector<Texture> textures;
        
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for(unsigned int j  = 0; j < textures_loaded.size(); j++ ){
                if(strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
                    textures.push_back(textures_loaded[j]);
                    cout << "Textura repetida" << endl;
                    skip = true;
                    break;
                }
            }

            if(!skip){
                Texture texture;
                texture.id = textureFromFile(str.C_Str(), directory, typeName);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
                cout << "PATH: " << texture.path << endl;
            }
            
        }

        return textures;
    }
    

    void Draw(Shader shader, unsigned int environmentMap = 0){
        for( unsigned int i = 0; i < meshes.size(); i++)
            if(environmentMap)
                meshes[i].Draw(shader, environmentMap);
            else 
                meshes[i].Draw(shader);
    }
};

unsigned int textureFromFile(const char *path, const string &directory, string typeName){
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    cout << "WIDTH: " << width << "     HEIGHT: " << height << endl;
    if(data){

        GLenum formatInt, format;

        if(nrComponents == 1){
            formatInt = GL_RED;
            format = GL_RED;
        }           
        // sRGB space
        else if(typeName == "texture_diffuse") 
            if (nrComponents == 3){
                formatInt = GL_SRGB;
                format = GL_RGB;
            }   
            else if (nrComponents == 4){
                formatInt = GL_SRGB_ALPHA;
                format = GL_RGBA;
            }
        // Linear Space
        else{
            if (nrComponents == 3){
                formatInt = GL_RGB;
                format = GL_RGB;
            }   
            else if (nrComponents == 4){
                formatInt = GL_RGBA;
                format = GL_RGBA;
            }
        }
              
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, formatInt, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    }
    else{
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}


#endif
