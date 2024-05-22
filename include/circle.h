#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>


class Circle {
	private:
		float p_x;
		float p_y;
	public: 
		void setPosition(float x, float y) {
			p_x = x;
			p_y = y;
		}
		float getX() {
			return p_x;
		}
		float getY() {
			return p_y;
		}

};