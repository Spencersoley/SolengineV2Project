#pragma once
#include <vector>
#include <InputManager.h>

class BeingManager;
class TransformSystem;

class SelectableSystem
{
public:
	SelectableSystem(
		TransformSystem& tformSys
	) :
		transformSystem(tformSys)
	{}

	uint32_t& getSelectedHandle() { return selectedHandle; }
	uint32_t getCSelectedHandle() const { return selectedHandle; }

	void processClick(const glm::vec2& mouseCoords, BeingManager& beings);

	void process(const BeingManager& beings) const;

	void clearSelectedHandle() { selectedHandle = UINT_MAX; }

private:
	TransformSystem& transformSystem;

	uint32_t selectedHandle{ UINT_MAX };
};