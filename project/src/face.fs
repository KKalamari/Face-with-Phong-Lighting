#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{    
    //sample the texture
    vec3 texColor = texture(material.diffuse, TexCoords).rgb;
    vec3 texSpecular = texture(material.specular, TexCoords).rgb;

    //ambient lighting
    vec3 ambient = light.ambient * texColor;

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texColor;

    //specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texSpecular;

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //apply attenuation to lighting
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //combine lighting effects
    vec3 finalColor = ambient + diffuse + specular;
    FragColor = vec4(finalColor, 1.0);
}

//recources https://learnopengl.com/Lighting/Basic-Lighting