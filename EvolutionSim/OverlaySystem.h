#pragma once
#include <Vertex.h>

class BeingManager;
class OverlayConfig;

class OverlaySystem
{
public:
	void updateOverlay(BeingManager& beings, OverlayConfig& overlayConfig) const;
};
