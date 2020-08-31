#pragma once

enum class OverlayMode 
{
	DEFAULT, 
	AGGRESSION, 
	DIET, 
	HEALTH, 
	HUNGER, 
	INTELLIGENCE, 
	SPEED, 
	STAMINA, 
	STRENGTH, 
	N_OF_MODES
};

class OverlayComponent
{
	template <typename T> friend class OverlaySystemInterface;
	OverlayMode mode{ OverlayMode::DEFAULT };
};