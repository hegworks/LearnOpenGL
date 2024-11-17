#version 330 core

// in
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    //FragColor = vec4(ioTexCoord,0,0);
    FragColor = texture(texture_diffuse1, ioTexCoord);
}