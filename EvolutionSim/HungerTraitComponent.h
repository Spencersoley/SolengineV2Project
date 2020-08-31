#pragma once

struct HungerTraitComponent
{
	template<class T> friend class HungerTraitSystemInterface;
	float value{ 0.5f };
	float fullness{ 0.0f };
};