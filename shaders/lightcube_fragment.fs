#version 330 core
out vec4 FragColor;
// Uniforms are variables that can be set on the cpp code and acessed by the GLSL code.
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
 //FragColor = texture(ourTexture, TexCoord);// * vec4(ourColor, 1.0);
	FragColor = vec4(lightColor, 1.0)*(texture(ourTexture,TexCoord)*2); 
}
