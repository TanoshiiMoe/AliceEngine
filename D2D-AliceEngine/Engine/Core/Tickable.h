#pragma once
class ITickable
{
public:
	virtual ~ITickable() = default;
	virtual void Update(const float& deltaSeconds) = 0;
};

