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
	//���ʹ�þ��������������ƫ�����ؽ��в���
	if (chaos || shake){
		for (int i=0; i<9; i++){
			sample[i] = vec3(texture(scene, texCoord.st + offsets[i]));
		}
	}

	//������Ч
	if (chaos){//��Ե���
		for (int i=0; i<9; i++){
			FragColor += vec4(sample[i] * edge_kernel[i], 0.0f);
			FragColor.a = 1.0f;
		}
	}else if (confuse){//��ɫ
		FragColor = vec4(1.0 - texture(scene, texCoord).rgb, 1.0);
	}else if (shake){//ģ��
		for (int i=0; i<9; i++){
			FragColor += vec4(sample[i] * blur_kernel[i], 0.0f);
			FragColor.a = 1.0f;
		}
	}else{
		FragColor = texture(scene, texCoord);
	}
};