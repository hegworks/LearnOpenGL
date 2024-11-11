#version 330 core

// in
in vec3 ioColor;
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main()
{
    FragColor = texture(uTexture0, ioTexCoord) * vec4(ioColor,1.0);
    FragColor = mix(texture(uTexture0, ioTexCoord),texture(uTexture1, vec2(ioTexCoord.x,-ioTexCoord.y)),0.5);
}