#pragma once
#include <glm\ext\vector_float3.hpp>
#include "StatSheet.h"

struct DuplicationData
{
	DuplicationData(glm::vec3 _pos, StatSheet _stats) 
		: 
		pos(_pos),
		stats(_stats)
	{}
	glm::vec3 pos{};
	StatSheet stats{};
};