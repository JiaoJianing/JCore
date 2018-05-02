#version 330 core					
layout (location=0) in vec4 vertex; //2 pos 2 uv

uniform mat4 model;
uniform mat4 projection;

out vec2 texCoord;

void main()						
{
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
	texCoord = vertex.zw;
};