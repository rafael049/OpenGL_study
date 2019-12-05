#version 430 core

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;


uniform vec3 viewPos;
uniform samplerCube skybox;

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

uniform int nPointLights;
uniform int nSpotLights;

struct Material{
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    
    float shininess;
    float specular;

    bool reflection;
    float reflectLevel;
};

uniform Light light;
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec4 calcShadding(vec3 lightAmb, vec3 lightDiff, vec3 lightSpec, vec3 lightDir, vec3 viewDir, float intensity, float attenuation);

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 calcReflection();

void main(){

    // check if the alpha value
    if(texture(material.texture_diffuse1, texCoord).a < 0.5){
        discard;
    }
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
    
    // Vectors
    vec3 norm = normal;
    vec3 viewDir = normalize(viewPos - fragPos);

    result = calcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < nPointLights; i++ ){
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    /*for(int i = 0; i < nSpotLights; i++){
        result += calcSpotLight(spotLights[i], norm, fragPos, viewDir);
    }*/
   
    if(material.reflection){
        vec3 I = normalize(fragPos - viewPos);
        vec3 R = reflect(I, normalize(normal));
        
        result = mix( result, vec4(texture(skybox, R).rgb, 1.0), material.reflectLevel );
    }


    // Output
    fragColor = result;

}

vec4 calcShadding(vec3 lightAmb, vec3 lightDiff, vec3 lightSpec, vec3 lightDir, vec3 viewDir, float intensity, float attenuation){
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfVect = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfVect), 0.0), material.shininess);

    vec4 ambient  = vec4(lightAmb , 1.0) * texture(material.texture_diffuse1, texCoord);
    vec4 diffuse  = vec4(lightDiff , 1.0) * diff * texture(material.texture_diffuse1, texCoord);
    vec4 specular = material.specular*vec4(lightSpec, 1.0) * spec * texture(material.texture_specular1, texCoord);

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return vec4((ambient + diffuse + specular).xyz, 1.0);
}

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    return calcShadding(light.ambient, light.diffuse, light.specular, lightDir, viewDir, 1.0, 1.0);    
}

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return calcShadding(light.ambient, light.diffuse, light.specular, lightDir, viewDir, attenuation, 1.0);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - fragPos);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 /(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotLight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return calcShadding(light.ambient, light.diffuse, light.specular, lightDir, viewDir, attenuation, intensity);
}

vec4 calcReflection(){
    vec3 I = normalize(fragPos - viewPos);
    vec3 R = reflect(I, normalize(normal));
    
    return vec4(texture(skybox, R).rgb, 1.0);
}