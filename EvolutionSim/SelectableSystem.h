#pragma once
#include <glm\ext\vector_float2.hpp>

class BeingManager;
struct SelectionBox;
class TransformSystem;

class SelectableSystem
{
	using Handle = unsigned int;

public:
	SelectableSystem(
		TransformSystem& tformSys
	) :
		transformSystem(tformSys)
	{}

	// fix this with underlying cast in  overloaded function
	Handle& getSelectedHandle() { return selectedHandle; }
	Handle  getCSelectedHandle() const { return selectedHandle; }

	void processClick(const glm::vec2& mouseCoords, BeingManager& beings);

	void update(const BeingManager& beings, SelectionBox& selectionBox) const;

	void clearSelectedHandle() { selectedHandle = UINT_MAX; }

private:
	TransformSystem& transformSystem;

	Handle selectedHandle{ UINT_MAX };
};