#include "Sphere.h"

const float PI = 3.14159265359f;
const float TAU = 6.28318530717f;


Sphere::Sphere(unsigned int xSegments, unsigned int ySegments)
{

    Vertex vertex;
    glm::vec3 vector;

    for (unsigned int y = 0; y <= ySegments; ++y)
    {
        for (unsigned int x = 0; x <= xSegments; ++x)
        {

            Vertex vertex;
            glm::vec3 vector;

            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;
            float xPos = std::cos(xSegment * TAU) * std::sin(ySegment * PI); // TAU is 2PI
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * TAU) * std::sin(ySegment * PI);


            vector.x = xPos;
            vector.y = yPos;
            vector.z = zPos;

            vertex.Position = vector;
            vertex.TexCoords = glm::vec2(xSegment, ySegment);
            vertex.Normal = vector;

            vertices.push_back(vertex);

        }
    }

    bool oddRow = false;
    for (int y = 0; y < ySegments; ++y)
    {
        for (int x = 0; x < xSegments; ++x)
        {
            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);

            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);
            Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
        }
    }


    // You must implement this function!
    setupSphere();
}


void Sphere::setupSphere()
{
    // generate a vertex array object (vao) to store the sphere's attributes
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // bind the vao so it's active

    // generate a vertex buffer object (vbo) to store vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vbo
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // send vertex data to gpu, static draw since it won't change often

    
    glGenBuffers(1, &EBO);// generate an element buffer object (ebo) to store index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the ebo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);// send index data to gpu, static draw since it won't change often

    // enable and define position attribute (index 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position)); // position attribute consists of 3 floats (x, y, z)
    
    glEnableVertexAttribArray(1);// enable and define normal attribute (index 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));  // normal attribute consists of 3 floats (x, y, z)
   
    glEnableVertexAttribArray(2); // enable and define texture coordinate attribute (index 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));// texture coordinates consist of 2 floats (u, v)

    // unbind the vao to prevent accidental modification
    glBindVertexArray(0);
}

void Sphere::Draw()
{
    glBindVertexArray(VAO); // bind the vao to set up the sphere for drawing
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    // draw the sphere using indexed drawing
    glBindVertexArray(0); // unbind the vao after drawing
}

//inspiration from https://www.songho.ca/opengl/gl_sphere.html