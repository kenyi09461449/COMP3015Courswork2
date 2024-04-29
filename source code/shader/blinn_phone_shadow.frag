#version 450
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Material {
    vec3 color;
    bool diffuse_exist;
    sampler2D diffuse;
    bool normal_exist;
    sampler2D normal;
    bool specular_exist;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
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

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform Material material;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse;
    if(material.diffuse_exist){
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    }
    else{
        diffuse = light.diffuse * diff * material.color;
    }
    
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);  
    return (ambient + (1.0-shadow)*(diffuse + specular));
    //return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * (texture(material.diffuse, fs_in.TexCoords)).rgb;
    //vec3 diffuse = light.diffuse * diff * vec3(1,1,1);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ( ambient + diffuse + specular);
}

void main()
{   
    // if(length((fs_in.FragPos)-vec3(0,0,0))<0.05)
    //     enablePrintf();
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result;
    
    result += CalcDirLight(dirLight, norm, viewDir);
    //result += CalcPointLight(pointLight, norm, fs_in.FragPos, viewDir);
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    //printf("from \"pixel\" %^2f! starting raymarch with world position %^3g , closesDepth %^1g,currentDepth %^1g, result %^3g...\n", gl_FragCoord.xy, fs_in.FragPos,closestDepth,currentDepth,result);
    FragColor = vec4(result, 1.0f);
}