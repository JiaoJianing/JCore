#version 330 core					
layout (location=0) in vec4 vertex; //2 pos 2 uv

uniform mat4 projection;
uniform vec2 offset;

out vec2 texCoord;

void main()						
{
	float scale = 10.0f;
	texCoord = vertex.zw;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
};