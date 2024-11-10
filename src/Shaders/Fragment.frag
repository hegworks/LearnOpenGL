#version 330 core

// in
in vec3 ourColor;

// out
out vec4 fragColor;

// uniform
uniform vec4 uniColor;

void main()
{
    fragColor = vec4(ourColor,1.0);
    //fragColor = uniColor;
}