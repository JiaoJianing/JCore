#version 330 core
out vec4 FragColor;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

in vec2 texCoord;

void main()
{
	FragColor = vec4(0.0f);
	vec3 sample[9];
	//如果使用卷积矩阵，则对纹理的偏移像素进行采样
	if (chaos || shake){
		for (int i=0; i<9; i++){
			sample[i] = vec3(texture(scene, texCoord.st + offsets[i]));
		}
	}

	//处理特效
	if (chaos){//边缘检测
		for (int i=0; i<9; i++){
			FragColor += vec4(sample[i] * edge_kernel[i], 0.0f);
			FragColor.a = 1.0f;
		}
	}else if (confuse){//反色
		FragColor = vec4(1.0 - texture(scene, texCoord).rgb, 1.0);
	}else if (shake){//模糊
		for (int i=0; i<9; i++){
			FragColor += vec4(sample[i] * blur_kernel[i], 0.0f);
			FragColor.a = 1.0f;
		}
	}else{
		FragColor = texture(scene, texCoord);
	}
};