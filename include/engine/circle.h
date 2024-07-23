#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Circle {
	public: 
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		float size = 0.0;
		Circle(glm::vec3 new_position, float new_size) {
			position = new_position;
			size = new_size;
		}
};