#version 330 core

// in
in vec3 ourColor;

// out
out vec4 fragColor;

void main()
{
    fragColor = vec4(ourColor,1.0);
}