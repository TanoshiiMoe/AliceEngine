#pragma once
#include <Component/Component.h>

/*
*	����� ������Ʈ �Դϴ�.
*	fmod ����, dll�� �ϴ� ���� �߰� ����
*/

class AudioComponent : public Component
{
public:
	AudioComponent();
	~AudioComponent();

	void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;

};

