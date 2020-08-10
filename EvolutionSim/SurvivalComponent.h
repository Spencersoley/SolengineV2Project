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
	template <class T> friend class SurvivalSystemInterface;

	float fullness{ 0.0f };
	float energy{ 0.0f };
	SurvivalState state{ SurvivalState::AWAITING };
	bool isAlive{ true };
};