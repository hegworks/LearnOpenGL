#version 330 core

// in
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, ioTexCoord);
}