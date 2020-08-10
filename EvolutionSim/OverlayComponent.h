#pragma once

enum class OverlayMode { DEFAULT, STRENGTH, SPEED, STAMINA, DIET, HUNGER };

class OverlayComponent
{
	template <typename T> friend class OverlaySystemInterface;
	OverlayMode mode{ OverlayMode::DEFAULT };
};