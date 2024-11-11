#version 330 core

// in
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iTexCoord;

// out
out vec3 ioColor;
out vec2 ioTexCoord;

// uniform
uniform mat4 uTransform = mat4(1.0f);

void main()
{
   gl_Position = uTransform * vec4(iPos, 1.0);
   ioColor = iColor;
   ioTexCoord = iTexCoord;
}