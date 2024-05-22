#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

   Shader mainShader("/home/falkun/Graphics/FsGe/shaders/circle_vertex.vs","/home/falkun/Graphics/FsGe/shaders/circle_fragment.fs");

   // x,y,z,r,g,b, texture_coord_y, texture_coord,y

    float square_vertices[] = {
	// Square
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f 
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. then set our vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // A stride of 7 floats
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
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


    // render loope 
    // -----------
    while (!glfwWindowShouldClose(window))
    {
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


        glBindVertexArray(VAO); // Since we only have one we can bind it here


        mainShader.use();
        mainShader.set2f("resolution", SCR_WIDTH, SCR_HEIGHT);
        mainShader.set2f("position", 0.5f, 0.5f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // If the escape key is pressed, close the window.
        glfwSetWindowShouldClose(window, true);
        std::cout << "Close Window Action Pressed" << std::endl; 
    } // Takes the window that should close as a parameter
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    	std::cout << "Marcos" << std::endl;
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