#pragma once

struct IntelligenceTraitComponent
{
	template <class T> friend class IntelligenceSystemInterface;
	float value{ 0.5f };
};