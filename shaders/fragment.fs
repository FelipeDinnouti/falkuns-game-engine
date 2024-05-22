#version 330 core
out vec4 FragColor;
//"uniform vec4 ourColor;\n" // Uniforms are variables that can be set on the cpp code and acessed by the GLSL code.
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;


void main()
{
 FragColor = texture(ourTexture, TexCoord);// * vec4(ourColor, 1.0);
 //FragColor = vec4(TexCoord.xy, 0.0, 0.0);
}
