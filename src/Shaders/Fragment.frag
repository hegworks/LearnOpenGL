#version 330 core

// in
in vec3 ioColor;
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D uTexture;

void main()
{
    FragColor = texture(uTexture, ioTexCoord) * vec4(ioColor,1.0);
}