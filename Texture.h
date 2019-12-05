#ifndef TEXTURE_H
#define TEXTURE_H



#include "glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

class Texture{
public:
    int width, height, nrChannels;
    unsigned int ID;

    Texture(const char* imagePath, unsigned int textureWrap, unsigned int textureFilter, bool alphaChannel, bool flipV = false ){
        // Load data from disk
        if(flipV)
            stbi_set_flip_vertically_on_load(true);

        unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if(!data){
            std::cout << "ERROR: Failed to load image: " << imagePath << std::endl;
        }

        // Create Textures
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Set wrap and filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);

        // Move data to texture
        if(alphaChannel)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

    }

    Texture(){
        
    }

    void setTextureUnit(GLenum unit){
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};



#endif
