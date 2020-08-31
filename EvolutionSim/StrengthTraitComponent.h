#pragma once

class StrengthTraitComponent
{
	template<class T> friend class StrengthTraitSystemInterface;
	float value{ 0.5f };
};