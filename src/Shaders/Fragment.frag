#version 330 core

// in
in vec3 vertexPos;

// out
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexPos,1.0);
}