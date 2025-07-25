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

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

private:
	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	/*	����
	float m_opacity = 1.0f;      // ��: ���̵� ȿ��
	FVector2 m_startPos;         // ��: �̵� �ִϸ��̼�
	FVector2 m_targetPos;
	float m_moveTime = 0.0f;     // �ִϸ��̼� �ð� ���
	float m_totalTime = 1.0f;    // �� �ִϸ��̼� �ð�
	*/
};

