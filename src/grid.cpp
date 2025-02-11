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

#include <chrono>
#include <thread>

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

// The screen size is based on the cell size.
int cell_size = 16;// In pixels
int grid_width = 40;
int grid_height = 40;

int getLiveNeighbours(int** grid, int x, int y) { //y*width+x
        int count = grid[x+1][y+1]+
                    grid[x][y+1]+
                    grid[x-1][y+1]+
                    grid[x+1][y]+
                    grid[x-1][y]+
                    grid[x+1][y-1]+
                    grid[x][y-1]+
                    grid[x-1][y-1];
        return count;
}

int main()
{
    // Creating the grid:
    float grid_width_float = static_cast<float>(grid_width);
    float grid_height_float = static_cast<float>(grid_width);


    float cell_size_ndc_x = 1.0/grid_width;
    float cell_size_ndc_y = 1.0/grid_height;
    int grid[grid_width][grid_height];

    int old_game_grid[grid_width+2][grid_height+2];
    int game_grid[grid_width+2][grid_height+2];

    int i;
    for (int x=0; x<=grid_width; x++) {
        for (int y=0; y<=grid_height; y++) {
            grid[x][y] = 0;
        }
    }

    

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
    GLFWwindow* window = glfwCreateWindow(grid_width*cell_size, grid_height*cell_size, "LearnOpenGL", NULL, NULL);
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

   Shader mainShader("/home/falkun/Remote/FsGe/shaders/grid_vertex.vs","/home/falkun/Remote/FsGe/shaders/grid_fragment.fs");

   // Initialization:
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSwapInterval(0);



    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         1.0f, -1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
         1.0f,  1.0f, 0.0f,  // top right
         -1.0f, 1.0f, 0.0f, // Top left
         -1.0f, -1.0f, 0.0f, // Down left
        1.0f,  1.0f, 0.0f  // top right
    };

    float grid_vertices[] = {
        cell_size_ndc_x, -cell_size_ndc_y, 0, // Bottom right
        -cell_size_ndc_x, -cell_size_ndc_y, 0, // Bottom left
        cell_size_ndc_x, cell_size_ndc_y, 0, // Top right
        -cell_size_ndc_x, cell_size_ndc_y, 0, // Top left
        -cell_size_ndc_x, -cell_size_ndc_y, 0, // Down left
        cell_size_ndc_x, cell_size_ndc_y, 0 // Top right
    };

    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    int x_pos = 0;
    int y_pos = 0;
/*
    game_grid[10][10] = 1;
    game_grid[10][11] = 1;
    game_grid[10][12] = 1;

    game_grid[20][10] = 1;     
    game_grid[20][11] = 1;
    game_grid[20][12] = 1;

    game_grid[9][9] = 1;
    game_grid[9][10] = 1;
    game_grid[9][11] = 1;*/

    // GLider
    game_grid[1][33] = 1;
    game_grid[2][32] = 1;
    game_grid[3][32] = 1;
    game_grid[3][33] = 1;
    game_grid[3][34] = 1;



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Wait for 500 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int input;
        std::cin >> input;
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // input
        // -----
        processInput(window);

        // Processing

        for (int x = 0; x<(grid_width+2); x++) {
            for (int y = 0; y<(grid_height+2); y++) {
                old_game_grid[x][y] = game_grid[x][y];
            }
        }

        for (int x = 0; x<(grid_width+2); x++) {
            for (int y = 0; y<(grid_height+2); y++) {
                // 4 Rules:
                // A live cell dies if it has fewer than 2 live neighbours
                // A live cell with 2 or three live neighbours goes to the next generation
                // A live cell with more than 3 neighbours dies
                // A dead cell wil be brought back to life if it has exactaly three live neighbours
                int liveNeighbours = old_game_grid[x+1][y+1]+
                    old_game_grid[x][y+1]+
                    old_game_grid[x-1][y+1]+
                    old_game_grid[x+1][y]+
                    old_game_grid[x-1][y]+
                    old_game_grid[x+1][y-1]+
                    old_game_grid[x][y-1]+
                    old_game_grid[x-1][y-1];

                if (old_game_grid[x][y] == 1) { // If the cell is live
                    std::cout << liveNeighbours << std::endl;
                    if (liveNeighbours<2) { // Less than two neighbours
                        game_grid[x][y] = 0;
                    } else if (liveNeighbours < 4) {// Less than 4 neighbours 
                        game_grid[x][y] = 1;     
                    } else {
                        game_grid[x][y] = 0; // Dies
                    }
                } else if (liveNeighbours == 3) {
                    game_grid[x][y] = 1;
                }
            }    
        }

        for (int x = 1; x<(grid_width); x++) {
            for (int y = 1; y<(grid_height); y++) {
                grid[x][y] = game_grid[x][y];
            }
        }



        // render
        // ------   
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        mainShader.use();
        glBindVertexArray(VAO); 
        


        for (int x = 0; x<grid_width; x++) {
            for (int y = 0; y<grid_height; y++) {
                float x_ndc = (((x+0.5)/grid_width_float)-0.5)*2;
                float y_ndc = (((y+0.5)/grid_height_float)-0.5)*2;
                mainShader.set2f("offset", x_ndc, y_ndc);
                if (grid[x][y] == 0) {
                    mainShader.set3f("color", 0.0, 0.0, 0.0);
                } else {
                    mainShader.set3f("color", 1.0, 1.0, 1.0);
                }

                //std::cout << "x_ndc: " << x*20+y << std::endl;
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        //xstd::cout << "---" << std::endl;


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int x = 0; x<grid_width; x++) {
        for (int y = 0; y<grid_height; y++) {
            std::cout << "x: " << x << " y: " << y << " value:" << grid[x][y] << std::endl;
        }    
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // If the escape key is pressed, close the window.
        glfwSetWindowShouldClose(window, true);
        std::cout << "Close Window Action Pressed" << std::endl; 
    } // Takes the window that should close as a parameter
   
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}