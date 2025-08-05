#pragma once
#include "Component/ScriptComponent.h"
#include "Component/SkewTransform.h"
class LaneController : public ScriptComponent
{
public:
	enum ActionType {
		MIN = 0,	// �ּڰ����� ��������
		MAX,		// �ִ񰪿��� �ö󰥽�
		MOVEUP,		// �����϶� (FSM�� �������޿�)
		MOVEDOWN,	// �����϶� (FSM�� �������޿�)
		IDLE,		// ���⶧ (FSM�� �������޿�)
		END			// ����
	};

	int maxLane = 2;
	int minLane = -2;
	float changeSpeed = 0.5f;

	void SetLaneWidth(float _width);
	float GetLaneWidth();
	int GetLaneIndex();
	void MoveUp();
	void MoveDown();

	void AddAction(ObjectHandle _handle, std::function<void()> _action, ActionType _type) {
		if (ObjectHandler::GetInstance().IsValid(_handle)) {
			actions.push_back({ _type, _handle, _action });
		}
	}
	void RemoveAction(ObjectHandle _handle, ActionType _type);

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

private:
	struct Slot {
		ActionType type = END;
		ObjectHandle handle;
		std::function<void()> func;
	};

	SkewTransform* st;
	float laneWidth = 120.0f;
	int currLane = 0;

	float moveElipsedTime = 0.0f;
	float initZpos = 0.0f;

	ActionType currStat;
	ActionType prevStat;

	std::vector<Slot> actions;

	void ChangeRenderLayer();
	void BroadCast(ActionType _type);
	void Move(const float& deltaTime);
};


