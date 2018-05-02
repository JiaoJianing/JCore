#version 330 core					
layout (location=0) in vec4 vertex; //2 pos 2 uv

out vec2 texCoord;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()						
{
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
	vec2 uv = vertex.zw;
	if (chaos){//混沌效果
		float strength = 0.3;
		vec2 pos = vec2(uv.x + sin(time) * strength, uv.y + cos(time) * strength);
		texCoord = pos;
	}else if (confuse){//翻转场景
		texCoord = vec2(1.0 - uv.x, 1.0 - uv.y);
	}else{
		texCoord = uv;
	}

	if (shake){
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
};