#pragma once
#include "GeneComponent.h"
namespace SolengineV2
{
	class RandomNumberGenerator;
}

template <class Implementation>
class GeneSystemInterface
{
public:
	static       AggressionTraitComponent&   getAggressionTrait   (GeneComponent& component)       { return component.aggression;   }
	static       DietTraitComponent&         getDietTrait         (GeneComponent& component)       { return component.diet;         }
	static       HealthTraitComponent&       getHealthTrait       (GeneComponent& component)       { return component.health;       }
	static       HungerTraitComponent&       getHungerTrait       (GeneComponent& component)       { return component.hunger;       }
	static       IntelligenceTraitComponent& getIntelligenceTrait (GeneComponent& component)       { return component.intelligence; }
	static       SpeedTraitComponent&        getSpeedTrait        (GeneComponent& component)       { return component.speed;        }
	static       StaminaTraitComponent&      getStaminaTrait      (GeneComponent& component)       { return component.stamina;      }
	static       StrengthTraitComponent&     getStrengthTrait     (GeneComponent& component)       { return component.strength;     }	
	static const AggressionTraitComponent&   getAggressionTrait   (const GeneComponent& component) { return component.aggression;   }
	static const DietTraitComponent&         getDietTrait         (const GeneComponent& component) { return component.diet;         }
	static const HealthTraitComponent&       getHealthTrait       (const GeneComponent& component) { return component.health;       }
	static const HungerTraitComponent&       getHungerTrait       (const GeneComponent& component) { return component.hunger;       }
	static const IntelligenceTraitComponent& getIntelligenceTrait (const GeneComponent& component) { return component.intelligence; }
	static const SpeedTraitComponent&        getSpeedTrait        (const GeneComponent& component) { return component.speed;        }
	static const StaminaTraitComponent&      getStaminaTrait      (const GeneComponent& component) { return component.stamina;      }
	static const StrengthTraitComponent&     getStrengthTrait     (const GeneComponent& component) { return component.strength;     }

	static void setFamilyHistory(GeneComponent& gene, TriQueue<unsigned int> set) { gene.familyHistory = set; }
	static TriQueue<unsigned int>& getFamilyHistory(GeneComponent& gene) { return gene.familyHistory; }
	static const TriQueue<unsigned int>& getFamilyHistory(const GeneComponent& gene) { return gene.familyHistory; }

	static void updateFamilyHistory(GeneComponent& component)
	{
		static Implementation system;
		system.updateFamilyHistory(component);
	}

	static void setAllTraits(GeneComponent& component, const float set)
	{
		static Implementation system;
		system.setAllTraits(component, set);
	}

	static void mutate(GeneComponent& gene, SolengineV2::RandomNumberGenerator& rng)
	{
		static Implementation system;
		system.mutate(gene, rng);
	}

	static bool isSharedFamilyHistory(const GeneComponent& geneA, const GeneComponent& geneB)
	{
		static Implementation system;
		return system.isSharedFamilyHistory(geneA, geneB);
	}
};


