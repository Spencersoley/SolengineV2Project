#pragma once
#include "IntelligenceTraitComponent.h"
#include "Handle.h"
struct GameData;

template <class Implementation>
class IntelligenceTraitSystemInterface
{
public:
	static void setTraitValue(IntelligenceTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const IntelligenceTraitComponent& component) { return component.value; }
};