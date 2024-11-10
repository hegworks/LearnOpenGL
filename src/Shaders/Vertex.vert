#version 330 core

// in
layout (location = 0) in vec3 aPos;

// out
out vec3 vertexPos;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   vertexPos = aPos;
}