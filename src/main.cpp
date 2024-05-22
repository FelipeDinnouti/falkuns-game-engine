#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Defining functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastTime;
float deltaTime;

// Camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront    = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection= glm::vec3(0.0f, 0.0f, 0.0f);
float cameraYaw   = 0.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 5.0f;

// Mouse
double mouse_lastX = SCR_WIDTH/2, mouse_lastY = SCR_HEIGHT/2, mouse_x=SCR_WIDTH/2, mouse_y=SCR_HEIGHT/2;
const float sensitivity = 0.1f;

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

   // x,y,z,r,g,b, texture_coord_y, texture_coord,y

    float square_vertices[] = {
	// Cube
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
	float triangle_vertices[] = { 
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f 
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

	unsigned int indices[] = { // All the elements inside this array are unsigned integers
		0, 1, 3, 1, 2, 3
	};

	unsigned int EBO, VBO, VAO; // Creating the objects. The unsigned int is the inique id that refers to the object.
	
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO); // Stores the VBO's name in the VBO unsigned int.
	glGenVertexArrays(1, &VAO);

	//1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
	// 3.  EBOS only work with things that aren't triangles i guess
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Not using EBO's rn because I don't have the indices for the cube
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. then set our vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // A stride of 7 floats
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	// glEnableVertexAttribArray(1); // enables the attribute

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); // enables the attribute

	// glVertexAttribPointer is a very important function, it basically specifies how the data should be stored.
	// And is what we use to send data to the shader
	// The first parameter is the location or index, which is the location that is later used in the shader (location = 1)
	// The second parameter is the size (can either be 1,2,3 or 4)
	// The 4th paramter is if the values shouldbe clamped (-1 .. 1 or 0..1 for unsigned)
	// The 5th paramter is the stride, the stride can be 0 if values are tightly packed
	// THe 6th parameter is basically where the value starts? its kinda confusing to me tbh

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Uncomment this line for wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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

        // process
        double time_value = glfwGetTime();
        float color_value = static_cast<float>(sin(time_value)/2.0f)+0.5f;

        // Bind the cube VAO and select the shader.
        glBindVertexArray(VAO); // Since we only have one we can bind it here
        mainShader.use();

        // ---- Doing the transformations
          
        // converts world space to view-space (what the camera sees with no projection)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPosition, cameraPosition+cameraFront, cameraUp); // Position, target and up

        glBindTexture(GL_TEXTURE_2D, frieren_texture);


        int i;
        for (i=0; i<10; i++)
        {
            //is what converts local space to world space
            glm::mat4 model = glm::mat4(1.0f);
            // Actually perform an rotation
            model = glm::translate(model, cubePositions[i]); // Offsets the model by the position in the list
            model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(0.5, 1.0, 0.0));
            model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.5, 1.0, 0.0));

            mainShader.setMatrix4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        mainShader.setMatrix4("view", view);
        mainShader.setMatrix4("projection", projection);

        // std::cout << (float)glfwGetTime() << std::endl; // prints current time

        // mainShader.setMatrix4("transform", trans); // Best way to send matrix
        
        //unsigned int transformLoc = glGetUniformLocation(mainShader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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

    cameraYaw += delta_x;
    cameraPitch += delta_y;

    mouse_lastX = xpos;
    mouse_lastY = ypos;

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
    	cameraPosition += cameraFront* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= cameraFront* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp))* cameraSpeed *deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp))* cameraSpeed *deltaTime;
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