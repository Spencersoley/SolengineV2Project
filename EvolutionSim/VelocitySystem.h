#pragma once
#include <glm\ext\vector_float2.hpp>

class BeingManager;
class GenerationManager;

class VelocitySystem
{
public:
	void update(BeingManager& beings, const GenerationManager& generationManager, unsigned int dt) const;
};
