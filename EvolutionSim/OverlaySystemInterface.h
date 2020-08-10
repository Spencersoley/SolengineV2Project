#pragma once
#include "OverlayComponent.h"

class BeingManager;
class OverlayConfig;

template <class Implementation>
class OverlaySystemInterface
{
public:
	static OverlayMode getOverlay(const OverlayComponent& overlay) { return overlay.mode; }
	static void setOverlay(OverlayComponent& overlay, OverlayMode set) { overlay.mode = set; }

	static void updateOverlay(BeingManager& beings, OverlayConfig& overlayConfig)
	{
		static Implementation system;
		system.updateOverlay(beings, overlayConfig);
	};
};

