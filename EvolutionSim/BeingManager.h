#pragma once
#include <vector>

#include "Vertex.h"
#include "Being.h"

class GeneSystem;
class SpriteSystem;
class VelocitySystem;
class SurvivalSystem;
class TransformSystem;

struct BeingCreateInfo;
struct Being;

class BeingManager
{
	using Colour = SolengineV2::Colour;
	using Handle = unsigned int;

public:
	BeingManager(
		const GeneSystem& geneSys,
		const SpriteSystem& spriteSys,
		const VelocitySystem& velSys,
		const SurvivalSystem& survSys,
		const TransformSystem& tformSys
	) :
		geneSystem(geneSys),
		spriteSystem(spriteSys),
		velocitySystem(velSys),
		survivalSystem(survSys),
		transformSystem(tformSys)
	{}

	size_t getSize() const;
	void init(const size_t size);
	void deleteBeing(const Handle handle, Handle& selectedHandle);
	void resize(const Handle start);
	void setToSize(const size_t size);
	void clear();
	void reset();

	std::vector<Being> pool{};

private:
	const GeneSystem& geneSystem;
	const SpriteSystem& spriteSystem;
	const VelocitySystem& velocitySystem;
	const SurvivalSystem& survivalSystem;
	const TransformSystem& transformSystem;
};