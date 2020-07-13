#include <glm\ext\vector_float3.hpp>

static inline glm::vec3 randomPosOutsideRadius(float minRadius) 
{
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float angle = (r1 * 2.0f - 1.0f) * static_cast<float>(M_PI);
	float radius = static_cast<float>(sqrt(static_cast<double>(r2) * static_cast<double>(30.0f))) + minRadius;
	return { radius * cos(angle), radius * sin(angle), 0.0f };
}

static inline glm::vec3 randomPosWithinRadius(float maxRadius) 
{
	float r1 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
	float r2 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
	float angle = (r1 * 2.0f - 1.0f) * static_cast<float>(M_PI);
	float radius = static_cast<float>(sqrt(r2)) * maxRadius * 0.9f;
	return { radius * cos(angle), radius * sin(angle), 0.0f };
}