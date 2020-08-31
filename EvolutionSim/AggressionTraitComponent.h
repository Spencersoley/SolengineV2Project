#pragma once

struct AggressionTraitComponent
{
	template <class T> friend class AggressionSystemInterface;
	float value{ 0.5f };
};