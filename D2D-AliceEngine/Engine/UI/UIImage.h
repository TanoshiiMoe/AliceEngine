#pragma once
#include <Component/UIComponent.h>
#include <Animation/TextureLoader.h>

using namespace Microsoft::WRL;

struct ID2D1Bitmap1;
class UIImage : public UIComponent
{
public:
	UIImage();
	virtual ~UIImage();

	void LoadData(const std::wstring& path);
	void SetScale(const FVector2& _scale) { scale = _scale; }
	void SetScale(const float& _x, const float& _y) { scale = FVector2(_x, _y); }
	void SetScale(const float& _x) { scale = FVector2(_x, _x); }

	void SetPosition(const FVector2& pos);

private:
	/*	����
	float m_opacity = 1.0f;      // ��: ���̵� ȿ��
	FVector2 m_startPos;         // ��: �̵� �ִϸ��̼�
	FVector2 m_targetPos;
	float m_moveTime = 0.0f;     // �ִϸ��̼� �ð� ���
	float m_totalTime = 1.0f;    // �� �ִϸ��̼� �ð�
	*/
};