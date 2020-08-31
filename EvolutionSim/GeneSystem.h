#pragma once
class GeneComponent;
namespace SolengineV2
{
	class RandomNumberGenerator;
}

class GeneSystem
{
public:
	void mutate(GeneComponent& gene, SolengineV2::RandomNumberGenerator& rng) const;
	void setAllTraits(GeneComponent& gene, const float set) const;

	void updateFamilyHistory(GeneComponent& gene) const;
	bool isSharedFamilyHistory(const GeneComponent& geneA, const GeneComponent& geneB) const;
};