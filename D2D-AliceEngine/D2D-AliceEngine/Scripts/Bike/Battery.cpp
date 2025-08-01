#include "Battery.h"

void Battery::AddBattery(int _val)
{
	battery += _val;
	Math::clamp<int>(_val, minSize, maxSize);

	CheckStat();
}

void Battery::RemoveSlots(ObjectHandle& _handle, ActionType _type)
{
	while (true) {
		auto it = std::find_if(slots.begin(), slots.end(),
			[_handle, _type](const std::pair<ActionType, Slot>& kv)
			{ return (kv.first == _type && kv.second.handle == _handle); });

		if (it == slots.end())
			break;
		else
			slots.erase(it);
	}
}

std::string Battery::GetCurrentStat()
{
	switch (currStat)
	{
	case Battery::MIN:
		return "MIN";
		break;
	case Battery::LOW:
		return "LOW";
		break;
	case Battery::MAX:
		return "MAX";
		break;
	default:
		break;
	}

	return "NULL";
}

void Battery::Update()
{
	if (prevStat != currStat) {
		prevStat = currStat;

		switch (currStat)
		{
		case Battery::MIN:
			BroadCast(MIN);
			break;
		case Battery::LOW:
			BroadCast(LOW);
			break;
		case Battery::MAX:
			BroadCast(MAX);
			break;
		default:
			break;
		}
	}

}

void Battery::CheckStat()
{
	if (battery <= minSize)
		currStat = MIN;
	else if (battery <= lowSize)
		currStat = LOW;
	else if (battery >= maxSize)
		currStat = MAX;
}

void Battery::BroadCast(ActionType _type)
{
	for (auto it = slots.begin(); it != slots.end();) {
		if (it->first == _type) {
			if (ObjectHandler::GetInstance().IsValid(it->second.handle)) {
				it->second.func();
				it++;
			}
			else
				it = slots.erase(it);
		}
		else
			it++;
	}
}
