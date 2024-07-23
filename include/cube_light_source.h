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

class CubeLight {
	public: 
		glm::mat4 model = glm::mat4(1.0f); // Identity model matrix
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		float rotation = 0.0f;
		glm::vec3 rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		void rotate(float angle, glm::vec3 axis) {
			rotation = angle ;
			rotation_axis = axis;
		}

		glm::mat4 getModelMatrix() {
			model = glm::mat4(1.0f); // Resets the model
			model = glm::translate(model, position);
			model = glm::rotate(model, rotation, rotation_axis);
			return model;
		}
		CubeLight(glm::vec3 new_position, glm::vec3 new_rotation) {
			position = new_position;
		}

};