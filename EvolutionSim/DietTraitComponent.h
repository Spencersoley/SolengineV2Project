#pragma once

struct DietTraitComponent
{
	template<class T> friend class DietTraitSystemInterface;
	float value{ 0.5f };
};