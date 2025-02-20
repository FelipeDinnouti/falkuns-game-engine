#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor; // Finds the color in the VBO data
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
// This transposing and inversing has something todo with non uniform scaling and how that affects normals.
	Normal = mat3(transpose(inverse(model))) * aNormal; 
	TexCoord = aTexCoord;
}
