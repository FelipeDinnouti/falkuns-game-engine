#version 330 core
out vec4 FragColor;
//"uniform vec4 ourColor;\n" // Uniforms are variables that can be set on the cpp code and acessed by the GLSL code.
in vec3 ourColor;

uniform vec2 position; // Percentage of position 
uniform vec2 resolution;

void main()
{
 // Need a uniform way of creating the circle or else it will be stretched;

 // Distance = Position-Position2
 // CurrentFragmentPosition = gl_FragCoord.xy 
 // CurrentPosition = resolution*position 
 vec2 position = resolution*position; 
 float distance = abs(length(gl_FragCoord.xy-position)); // frag_coord  position
 
 if (distance < 10.0) {
 	FragColor = vec4(0.0,0.0,0.0, 1.0);
 } else {
 	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
 }
}
