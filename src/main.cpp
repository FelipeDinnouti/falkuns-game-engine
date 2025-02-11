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
std::list<Cube> cubeList;



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

   Shader mainShader("/home/falkun/Remote/FsGe/shaders/vertex.vs","/home/falkun/Remote/FsGe/shaders/fragment.fs");
   Shader lightcubeShader("/home/falkun/Remote/FsGe/shaders/vertex.vs","/home/falkun/Remote/FsGe/shaders/lightcube_fragment.fs");


   // Initialization:
   glEnable(GL_DEPTH_TEST);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);

    float plain_cube_data[] = {
	// Cube
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
 
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

	unsigned int cube_EBO, cube_VBO, cube_VAO, lightcube_EBO, lightcube_VBO, lightcube_VAO; // Creating the objects. The unsigned int is the inique id that refers to the object.
	

    // Generate the VAO for the cube cube
	glGenBuffers(1, &cube_EBO);
	glGenBuffers(1, &cube_VBO); // Stores the VBO's name in the VBO unsigned int.
	glGenVertexArrays(1, &cube_VAO);

	//1. bind Vertex Array Object
	glBindVertexArray(cube_VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plain_cube_data), plain_cube_data, GL_STATIC_DRAW);
	// 4. then set our vertex attributes pointers
	// Three floats for - Position Infomration - 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Stride of 8 floats
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); 

    // Three float for - Normal Information - 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    glEnableVertexAttribArray(2); 


	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

    // Generate the VAO for the LightCube
    glGenBuffers(1, &lightcube_EBO);
    glGenBuffers(1, &lightcube_VBO); // Stores the VBO's name in the VBO unsigned int.
    glGenVertexArrays(1, &lightcube_VAO);

    //1. bind Vertex Array Object
    glBindVertexArray(lightcube_VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, lightcube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plain_cube_data), plain_cube_data, GL_STATIC_DRAW);
    // 4. then set our vertex attributes pointers

    // Three floats for - Position Information - 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Stride of 5 floats
    glEnableVertexAttribArray(0);

    // Two floats for - Texture Coordinates - 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); // Don't forget to enable the attribute

    // Three float for - Normal Information - 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    glEnableVertexAttribArray(2); 

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);


    // --------- TEXTURE
    const  char* frieren_image_path = "frierensmug.jpeg";
    Texture frieren_texture = Texture(frieren_image_path);

    const  char* container_image_path = "container.jpg";
    Texture container_texture = Texture(container_image_path);
    
    glm::mat4 trans = glm::mat4(1.0f); // Identity matrix, which means there are 1's on the diagonal


    // Projection matrix is the matrix that adds perspective
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);


    // Initializing World Objects

    int i;
    for (i=0; i<11; i++) {
        cubeList.push_back(Cube(cubePositions[i], glm::vec3(0.0f, 0.0f, 0.0f)));
    }

    camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

    Cube lightCube = Cube(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    // render loope 
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //std::cout << 1/deltaTime << std::endl;

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
        float color_value = static_cast<float>(sin(time_value/4)/2.0f)+0.5f; // Some weird color variation

        // Bind the cube VAO and select the shader.
        glBindVertexArray(cube_VAO); // Since we only have one we can bind it here
        mainShader.use();   

        // ------ Camer a Logic
        glm::mat4 view = glm::mat4(1.0f);

        camera.Update(); // Calculates the direction of the camera
        view = customLookAt(camera.position, camera.direction);
        
        mainShader.setMatrix4("view", view);
        mainShader.setMatrix4("projection", projection);

        // Makes the camera look at the center of the world, good for debugging.
        camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); 
        
        // Lighting 
        mainShader.set3f("objectColor",  1.0f, 0.5f, 0.31f);
        mainShader.set3f("lightColor", 1.0f, 1.0f, 1.0f);
        mainShader.set3f("lightPos", lightCube.position.x,  lightCube.position.y,  lightCube.position.z);
        mainShader.set3f("viewPos", camera.position.x, camera.position.y, camera.position.z);

        // Binding an texture makes so it is in use for all the next drawing calls
        container_texture.bind();

        Cube testcube = cubeList.front();
        mainShader.setMatrix4("model", testcube.getModelMatrix()); // Esse funciona
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Light object
        glBindVertexArray(lightcube_VAO);

        lightcubeShader.use();
        lightcubeShader.set3f("lightColor", 1.0f, 1.0f, 1.0f);

        lightcubeShader.setMatrix4("view", view);
        lightcubeShader.setMatrix4("projection", projection);

        frieren_texture.bind();
        lightCube.position = glm::vec3(
            static_cast<float>(sin(time_value*0.4))*2.5, 
            static_cast<float>(cos(time_value*0.6))*1.5, 
            static_cast<float>(sin(time_value*0.5))*2);        

        //lightCube.position = glm::vec3(0.0f, 0.0f, 0.0f);

        lightCube.rotation -= deltaTime;
        lightCube.rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
        lightcubeShader.setMatrix4("model", lightCube.getModelMatrix()); // Esse não.
        glDrawArrays(GL_TRIANGLES, 0, 36);

       


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // There are two buffers: back buffer and front buffer. The front buffer is the one being rendered to the screen and the back buffer
        // Is where we make our changes. When we want to update the screen we swap the buffers.
    }

    // Optional: De allocate our created resources.
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &cube_VBO);
    glDeleteVertexArrays(1, &cube_EBO);

    glDeleteVertexArrays(1, &lightcube_VAO);
    glDeleteVertexArrays(1, &lightcube_VBO);
    glDeleteVertexArrays(1, &lightcube_EBO);

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