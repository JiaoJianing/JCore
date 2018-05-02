#version 330 core
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

in vec2 texCoord;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoord).r);
	FragColor = vec4(textColor, 1.0) * sampled;
};