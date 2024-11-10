#version 330 core

// in
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// out
out vec3 ourColor;

// uniform
uniform float horizontalOffset;
uniform int isVerticallyFlipped = 1;

void main()
{
   gl_Position = vec4(aPos.x + horizontalOffset,isVerticallyFlipped * aPos.y, aPos.z, 1.0);
   ourColor = aColor;
}