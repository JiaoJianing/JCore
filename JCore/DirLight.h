#pragma once
#include "BaseLight.h"
#include <glm.hpp>

//平行光 目前场景中只支持一个
class DirLight : public BaseLight
{
public:
	DirLight();
	~DirLight();
};

