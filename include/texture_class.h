#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
	public: 
		unsigned int texture_object;
		void bind() {
			glBindTexture(GL_TEXTURE_2D, texture_object);
		}

		Texture(const char* path) {
			// Create the texture
			glGenTextures(1, &texture_object);
			glBindTexture(GL_TEXTURE_2D, texture_object);

			// Some default options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		    int width, height, nrChannels;
		    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D); {}
			} else {
				std::cout << "Failed to load texture with path: " << path << std::endl;
			}
			stbi_image_free(data);
		}

};