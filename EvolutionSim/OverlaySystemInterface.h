#pragma once
#include "OverlayComponent.h"
struct GameData;

template <class Implementation>
class OverlaySystemInterface
{
public:
	static OverlayMode getOverlayMode(const OverlayComponent& overlay) { return overlay.mode; }
	static void setOverlay(OverlayComponent& overlay, OverlayMode set) { overlay.mode = set; }

	static void updateOverlay(GameData& gameData)
	{
		static Implementation system;
		system.updateOverlay(gameData);
	};
};