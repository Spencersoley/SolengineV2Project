#pragma once

struct HealthTraitComponent
{
	template <class T> friend class HealthSystemInterface;
	float value{ 0.5f };
	float currentHealth{ 0.0f };
};