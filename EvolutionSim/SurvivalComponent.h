#pragma once

class SurvivalComponent
{
public:
	enum class SurvivalState
	{
		AWAITING,
		SEARCHING,
		RETURNING,
	};

private:
	friend class SurvivalSystem;
	friend struct Being;

	float fullness{ 0.0f };
	float energy{ 0.0f };
	SurvivalState state{ SurvivalState::AWAITING };
	bool isAlive{ true };
};