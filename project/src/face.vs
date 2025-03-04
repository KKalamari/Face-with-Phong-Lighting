#version 330 core
layout (location = 0) in vec3 aPos; //pos of the vertex
layout (location = 1) in vec3 aNormal; //normal vector og the vertex
layout (location = 2) in vec2 aTexCoords; //texture coord

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); //cconverts the vertex position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; //ensures that normals are properly transformed when the model is scaled or rotated.
    TexCoords = aTexCoords; //passes texture coordinates to the fragment shader.
    gl_Position = projection * view * vec4(FragPos, 1.0);//rendering 
}