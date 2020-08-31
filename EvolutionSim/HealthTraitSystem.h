#pragma once
struct HealthTraitComponent;

class HealthTraitSystem
{
public:
	void depleteCurrentHealth(HealthTraitComponent& component, const float deplete)const;
	void setCurrentHealthToMax(HealthTraitComponent& component) const;
	float getHealthWhenFull(const HealthTraitComponent& component)const;
};