#pragma once

class StaminaTraitComponent
{
	template<class T> friend class StaminaTraitSystemInterface;
	float value{ 0.5f };
	float energy{ 0.0f };
};