#pragma once

enum class SurvivalState 
{
	AWAITING,
	SEARCHING,
	RETURNING,
};

class SurvivalComponent
{
public:
	SurvivalComponent() {};
	SurvivalComponent(
		const SurvivalState survivalState,
		const bool alive,
		float food,
		float stamina
	) : 
		state(survivalState),
		isAlive(alive),
		fullness(food),
		energy(stamina * 1000.0f)
	{}

private:
	friend class SurvivalSystem;
	friend class PartitionSystem; //access provided so that partition utilises survival component
	SurvivalState state{ SurvivalState::AWAITING };
	bool isAlive{ true };
	float fullness{ 0.0f };
	float energy{ 0.0f };
	bool exists{ true };
};