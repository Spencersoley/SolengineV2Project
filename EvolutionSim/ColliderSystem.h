#pragma once
#include <vector>

class SurvivalSystem;
class GeneSystem;
class PartitionSystem;
class BeingManager;
class TransformSystem;
class VelocitySystem;

enum class CollisionType
{
	ALIVE_ANIMAL_VS_ALIVE_ANIMAL = 1 << 1,
	ALIVE_ANIMAL_VS_OTHER = 1 << 2,
	OTHER_VS_ALIVE_ANIMAL = 1 << 3,
	OTHER = 1 << 4
};

class ColliderSystem
{
public:
	ColliderSystem(
		TransformSystem& tformSys,
		SurvivalSystem& survSys,
		GeneSystem& geneSys,
		const PartitionSystem& partSys,
		const VelocitySystem& velSys
	) :
		transformSystem(tformSys),
		survivalSystem(survSys),
		geneSystem(geneSys),
		partitionSystem(partSys),
		velocitySystem(velSys)
	{}

	void process(BeingManager& beings, long long dt);

private:
	TransformSystem& transformSystem;
	SurvivalSystem& survivalSystem;
	GeneSystem& geneSystem;
	const PartitionSystem& partitionSystem;
	const VelocitySystem& velocitySystem;

	const float foodStrengthMultiplier{ 0.00000001f };

	void detectCollision(BeingManager& beings, const uint32_t handleA, const uint32_t handleB, int dt);

	CollisionType detectCollisionType(BeingManager& beings, const uint32_t handleA, const uint32_t handleB);

	//void evaluateStrengthBonus(const GeneComponent& geneA, const GeneComponent& geneB, float& strengthBonusA, float& strengthBonusB)
	//{
	//	float strengthA = geneSystem.getTrait(geneA, Trait::STRENGTH);
	//	float strengthB = geneSystem.getTrait(geneB, Trait::STRENGTH);

	//	float strengthRatioAToB = strengthA / (strengthA + strengthB);

	//	strengthBonusA = strengthRatioAToB;
	//	strengthBonusB = (1 - strengthRatioAToB);
	//}
};
