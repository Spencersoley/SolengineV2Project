#pragma once
#include <Vertex.h>
struct GameData;

class OverlaySystem
{
public:
	void updateOverlay(GameData& gameData) const;
};