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

#include <cmath>

class Camera {
		glm::vec3 lookAt_target = glm::vec3(0.0f, 0.0f, 0.0f);
		bool lookAt_set = false;

	public: 
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

		void lookAt(glm::vec3 target) {
			lookAt_target = target;
			lookAt_set = true;
		}

		void Update() { // Direction = (sin(ry)*cos(rx), sin(rx), cos(rx)*cos(ry))
			if (lookAt_set == true) {
				direction = glm::normalize(lookAt_target-position);
				lookAt_set = false;
				return;
			}

			direction.x = cos(rotation.x)*cos(rotation.y);
			direction.y = sin(rotation.y);
			direction.z = sin(rotation.x)*cos(rotation.y);			
		}

		Camera(glm::vec3 new_position, glm::vec3 new_rotation) {
			position = new_position;
		}

};