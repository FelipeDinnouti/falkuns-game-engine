#version 330 core
out vec4 FragColor;
//"uniform vec4 ourColor;\n" // Uniforms are variables that can be set on the cpp code and acessed by the GLSL code.
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
// Note that most people tend to do all this calculations in view space because the viewer position
// is always 0,0,0 but here we are not for learning purpouses
uniform vec3 viewPos;

float specularStrength = 0.5f;

void main()
{
	// Calculating the diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff*lightColor;

	// Calculating the specular lighting.
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), 32);
	vec3 specular = specularStrength*spec*lightColor;

	float ambient_strength = 0.3;
	vec3 ambient = ambient_strength*lightColor;

	vec3 result = (ambient+diffuse+specular)*objectColor;
	FragColor = vec4(result, 1.0); 
}
