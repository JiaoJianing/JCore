#version 330 core
out vec4 FragColor;

uniform sampler2D sprite;
uniform vec3 spriteColor;

in vec2 texCoord;

void main()
{
	FragColor = vec4(spriteColor, 1.0) * texture(sprite, texCoord);
};