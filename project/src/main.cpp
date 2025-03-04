#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../../Linking/lib/camera.h"
#include "../../Linking/lib/shader.h"
#include "../../Linking/lib/model.h"
#include "../../models/sphere/Sphere.h"

//stores time of last frame update
float lastFrame = 0.0f;

// stores accumulated frame time
float frameToggled = 0.0f;

// sets window width and height
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// defines orbit radius of sphere
float sphereOrbitRadius = 10.0f;

//sets position of face model
glm::vec3 facePos = glm::vec3(0.0f, -1.0f, 0.0f);

// stores position of sphere
glm::vec3 spherePos;

// initializes camera at a specific position
Camera camera(glm::vec3(0.0f, 0.0f, 25.0f));

//defines movement speed modifier
float speedFactor = 1.0f;

// stores last known cursor position
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// flag to check if it's first mouse movement
bool firstMouse = true;

//function declarations for callbacks and input handling
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    // initializes glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Face with light", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // makes the window's opengl context current
    glfwMakeContextCurrent(window);

    // sets callback functions
   //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // loads glad to manage opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize glad" << std::endl;
        return -1;
    }

    // enables depth testing
    glEnable(GL_DEPTH_TEST);

    Shader sphereShader("./src/sphere.vs", "./src/sphere.fs");
    Shader headShader("./src/face.vs", "./src/face.fs");
    Model Face("./models/head/woman1.obj");
    Sphere MoonSphere(10, 10);

    // main render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculates time between frames
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // updates sphere orbit animation
        frameToggled += deltaTime * speedFactor;
        //screen with a dark gray color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // processes user input
        processInput(window, deltaTime);
        //creates projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // updates sphere position in circular orbit
        spherePos = facePos + glm::vec3(sin(frameToggled) * sphereOrbitRadius, 2.0f, cos(frameToggled) * sphereOrbitRadius);

        // lighting properties for face shader
        headShader.use();
        headShader.setVec3("light.position", spherePos);
        headShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        headShader.setVec3("light.diffuse", 1.2f, 1.2f, 1.2f);
        headShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // binds textures for face model
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Face.textures_loaded[0].id);
        headShader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Face.textures_loaded[1].id);
        headShader.setInt("material.specular", 1);

        // lighting parameters
        headShader.setFloat("light.constant", 1.0f);
        headShader.setFloat("light.linear", 0.07f);
        headShader.setFloat("light.quadratic", 0.017f);

        // material shininess
        headShader.setFloat("material.shininess", 32.0f);

        // transformations for face model
        headShader.setMat4("projection", projection);
        headShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, facePos);
        model = glm::scale(model, glm::vec3(0.045f));
        headShader.setMat4("model", model);
        Face.Draw(headShader);

        // renders sphere wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        sphereShader.use();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, spherePos);
        model = glm::scale(model, glm::vec3(0.5f));
        sphereShader.setMat4("model", model);
        MoonSphere.Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        speedFactor += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        speedFactor = std::max(0.05f, speedFactor - 0.05f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}