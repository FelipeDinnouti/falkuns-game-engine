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


// Defining functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void regenVoxelVertices();
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
int voxel_data[8] = {};
float voxel_vertices[512];
unsigned int indices[128];

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

   Shader mainShader("/home/falkun/Graphics/FsGe/shaders/vertex.vs","/home/falkun/Graphics/FsGe/shaders/fragment.fs");

    // Initialization:
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    float default_plane[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    float plane_indices[] = {
        0, 1, 3,
        1, 2, 3
    };

	unsigned int EBO, VBO, VAO; // Creating the objects. The unsigned int is the inique id that refers to the object.

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO); // Stores the VBO's name in the VBO unsigned int.
	glGenVertexArrays(1, &VAO);

	//1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(default_plane), default_plane, GL_DYNAMIC_DRAW);
	// 4. then set our vertex attributes pointers
	// position attribute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Not using EBO's rn because I don't have the indices for the cube
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // A stride of 7 floats
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

    // --------- TEXTURE
    unsigned int frieren_texture;
    glGenTextures(1, &frieren_texture);
    glBindTexture(GL_TEXTURE_2D, frieren_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("frierensmug.jpeg", &width, &height, &nrChannels, 0);
    if (data) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load asset frierensmug :(" << std::endl;
    }
    stbi_image_free(data);

    
    glm::mat4 trans = glm::mat4(1.0f); // Identity matrix, which means there are 1's on the diagonal


    // Projection matrix is the matrix that adds perspective
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);


    // Initializing World Objects
    int x,y;
    for (x=0; x<=7; x++) {
        int row_data[8] = {};
        for (y=0;y<=7; y++) {
            row_data[y] = 0;
        }
        voxel_data[x] = *row_data;
    }
    /*
    for (x=0; x<=7; x++) {
        for (y=0;y<=7; y++) {
            std::cout << "---\nX: " << x << "\nY: " << y << std::endl;
        }
    }
    */
    camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

    // render loope 
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //std::cout << 1/deltaTime << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clearing the depth buffer
        deltaTime = (float)glfwGetTime()-lastTime;
        lastTime = (float)glfwGetTime();

        // input
        // -----
        processInput(window);

        // render
        // ------

        glClearColor(0.3f, 0.1f, 0.3f, 1.0f); // Sets the color
        glClear(GL_COLOR_BUFFER_BIT); // Applies the color???
        // The screen should be a blank purple-ish color

        //regenVoxelVertices();

        // process
        double time_value = glfwGetTime();
        float color_value = static_cast<float>(sin(time_value)/2.0f)+0.5f;

        // Bind the cube VAO and select the shader.
        glBindVertexArray(VAO); // Since we only have one we can bind it here
        mainShader.use();

        // ---- Doing the transformations
          
        // converts world space to view-space (what the camera sees with no projection)
        glm::mat4 view = glm::mat4(1.0f);
        
        //std::cout << camera.rotation.y << std::endl;
        camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        camera.Update(); // Calculates the direction of the camera
        view = customLookAt(camera.position, camera.direction);
    
        // TODO: Create camera class with custom angles, to allow for X Y Z rotation like an normal object.

        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);

        mainShader.setMatrix4("view", view);
        mainShader.setMatrix4("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // There are two buffers: back buffer and front buffer. The front buffer is the one being rendered to the screen and the back buffer
        // Is where we make our changes. When we want to update the screen we swap the buffers.
    }

    // Optional: De allocate our created resources.
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
    glDeleteVertexArrays(1, &EBO);


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

void regenVoxelVertices() {
    int i,x,y, a;
    i = 0;
    a = 0;
    for (x=0; x<=7; x++) {
        for (y=0;y<=7; y++) {
            //std::cout << "---\nX: " << x << "\nY: " << y << std::endl;


            voxel_vertices[i] = (float)x+0.5;// x
            voxel_vertices[i+1] = (float)y+0.5;  

            voxel_vertices[i+2] = (float)x-0.5;// x
            voxel_vertices[i+3] = (float)y-0.5;

            voxel_vertices[i+4] = (float)x+0.5;// x
            voxel_vertices[i+5] = (float)y-0.5;

            voxel_vertices[i+6] = (float)x-0.5;// x
            voxel_vertices[i+7] = (float)y+0.5;


            indices[a] = i;
            indices[a+1] = i+1;
            indices[a+2] = i+2;
            indices[a+3] = i;
            indices[a+4] = i+3;
            indices[a+5] = i+1;
            a+= 6;

            i+=8;
        }
    }
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}