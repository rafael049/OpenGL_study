#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTex;

const float offset = 1.0 / 30.0;

void main(){

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,    
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];

    for(int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTex, texCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);

    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * kernel[i];
    }

   
    FragColor = pow(texture(screenTex, texCoords), vec4(0.45));
}