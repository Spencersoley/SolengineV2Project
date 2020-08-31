#pragma once
#include <climits>
#include "Handle.h"

class SelectedComponent
{
	template <class T> friend class SelectableSystemInterface;
	Handle beingHandle{ UINT_MAX };
	Handle foodHandle{ UINT_MAX };
};