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
	SurvivalState state{ SurvivalState::AWAITING };
	bool isPoisoned{ false };
};