#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bits/stdc++.h>
#include <list>
#include <iterator>

#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// My libraries
#include "cube.h"
#include "engine/camera.h"
#include "texture_class.h"
#include "engine/circle.h"


// Defining functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
glm::mat4 customLookAt(glm::vec3 position, glm::vec3 target);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastTime;
float deltaTime;

// Camera
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront    = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection= glm::vec3(0.0f, 0.0f, 0.0f);
float cameraYaw   = 1.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 5.0f;

// Mouse
double mouse_lastX = SCR_WIDTH/2, mouse_lastY = SCR_HEIGHT/2, mouse_x=SCR_WIDTH/2, mouse_y=SCR_HEIGHT/2;
const float sensitivity = 0.01f;

// World
std::list<Circle> circleList;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    std::cout << "GLFW Initializing" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Tells the version of OpenGL to GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // So this tells that the version is 3.3 (major.minor)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // And this tells we are using the Core Profile

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

   Shader mainShader("/home/falkun/Graphics/FsGe/shaders/fun_vertex.vs","/home/falkun/Graphics/FsGe/shaders/fun_fragment.fs");

   // Initialization:
   glEnable(GL_DEPTH_TEST);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);

    glm::mat4 trans = glm::mat4(1.0f); // Identity matrix, which means there are 1's on the diagonal

    // Projection matrix is the matrix that adds perspective
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
    
    // Create some circles
    circleList.push_back(Circle(glm::vec3(0.0f, 0.0f, 0.0f), 0.1f));

    // render loope 
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        std::cout << 1/deltaTime << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clearing the depth buffer
        deltaTime = (float)glfwGetTime()-lastTime;
        lastTime = (float)glfwGetTime();

        // ----- Input 
        processInput(window);

        // ------ Rendering
        glClearColor(0.1f, 0.06f, 0.1f, 1.0f); // Sets the color
        glClear(GL_COLOR_BUFFER_BIT); 

        // Time 
        double time_value = glfwGetTime();
      
        mainShader.use();   

        // ------ Camer a Logic
        glm::mat4 view = glm::mat4(1.0f);

        camera.Update(); // Calculates the direction of the camera
        view = customLookAt(camera.position, camera.direction);
        
        mainShader.setMatrix4("view", view);
        mainShader.setMatrix4("projection", projection);

        // Makes the camera look at the center of the world, good for debugging.
        camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); 
    


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // There are two buffers: back buffer and front buffer. The front buffer is the one being rendered to the screen and the back buffer
        // Is where we make our changes. When we want to update the screen we swap the buffers.
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Process mouse callback

void mouse_callback(GLFWwindow *window, double xpos, double ypos) 
{
    mouse_x = xpos;
    mouse_y = ypos;

    double delta_x = mouse_x-mouse_lastX;
    double delta_y = mouse_lastY-mouse_y; 

    delta_x *= sensitivity;
    delta_y *= sensitivity;

    camera.rotation.x += delta_x;
    camera.rotation.y += delta_y;

    mouse_lastX = xpos;
    mouse_lastY = ypos;

    //std::cout << cameraPitch << std::endl;
}

glm::mat4 customLookAt(glm::vec3 position, glm::vec3 target) {
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 translation = glm::mat4(1.0f);

    // Cross product of the world up vector and the direction vector
    glm::vec3 direction = glm::normalize(-target); 
    

    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    glm::vec3 up = glm::normalize(glm::cross(direction, right));

    rotation[0][0] = right.x;
    rotation[1][0] = right.y;
    rotation[2][0] = right.z;
    rotation[0][1] = up.x;
    rotation[1][1] = up.y;
    rotation[2][1] = up.z;
    rotation[0][2] = direction.x;
    rotation[1][2] = direction.y;
    rotation[2][2] = direction.z;

    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    return rotation*translation;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Camera Movement
    cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection.z = sin(glm::radians(cameraYaw));
    cameraDirection.y = sin(glm::radians(cameraPitch)) * cos(glm::radians(cameraPitch)); 
    cameraFront = glm::normalize(cameraDirection);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // If the escape key is pressed, close the window.
        glfwSetWindowShouldClose(window, true);
        std::cout << "Close Window Action Pressed" << std::endl; 
    } // Takes the window that should close as a parameter
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    	camera.position += camera.direction* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= camera.direction* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position -= glm::normalize(glm::cross(camera.direction, cameraUp))* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position += glm::normalize(glm::cross(camera.direction, cameraUp))* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.position -= glm::normalize(cameraUp)* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.position += glm::normalize(cameraUp)* cameraSpeed *deltaTime;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}