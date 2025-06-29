#pragma once
#include "SystemBase.h"
class TransformSystem : public SystemBase, public Singleton<TransformSystem>
{
public:
	TransformSystem() {}
	~TransformSystem() {}
};

