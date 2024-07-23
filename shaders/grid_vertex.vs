#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor;

uniform vec2 offset; 

void main()
{
    gl_Position = vec4(aPos.xy+offset, aPos.z, 1.0);
}

