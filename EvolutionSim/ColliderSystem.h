#pragma once
#include <set>

class BeingManager;

class GeneSystem;
class PartitionSystem;
class SpriteSystem;
class SurvivalSystem;
class TextureSystem;
class TransformSystem;
class VelocitySystem;

const float FOOD_STRENGTH_MULTIPLIER{ 0.00001f };

class ColliderSystem
{
	using Handle = unsigned int;

public:
	ColliderSystem(
		const GeneSystem& geneSys,
		PartitionSystem& partSys,
		SpriteSystem& spriteSys,
		SurvivalSystem& survSys,
		const TextureSystem& textureSys,
		TransformSystem& tformSys,
		const VelocitySystem& velSys
	) :
		geneSystem(geneSys),
		partitionSystem(partSys),
		spriteSystem(spriteSys),
		survivalSystem(survSys),
		textureSystem(textureSys),
		transformSystem(tformSys),
		velocitySystem(velSys)
	{}

	void update(BeingManager& beings, unsigned int dt);

	enum class CollisionType
	{
		ALIVE_ANIMAL_VS_ALIVE_ANIMAL = 1 << 1,
		ALIVE_ANIMAL_VS_OTHER = 1 << 2,
		OTHER_VS_ALIVE_ANIMAL = 1 << 3,
		OTHER = 1 << 4
	};

	std::set<Handle>& getToDelete(const BeingManager& beings);

private:
	const GeneSystem& geneSystem;
	PartitionSystem& partitionSystem;
	SpriteSystem& spriteSystem;
	SurvivalSystem& survivalSystem;
	const TextureSystem& textureSystem;
	TransformSystem& transformSystem;
	const VelocitySystem& velocitySystem;

	void detectCollision(Being& beingA, Being& beingB, const Handle handleA, const Handle handleB, const float foodMultiplier);

	void eatOnCollision(float foodMultiplier, Being& beingA, Being& beingB, const Handle handleB);

	std::set<Handle> toDelete{};
};
