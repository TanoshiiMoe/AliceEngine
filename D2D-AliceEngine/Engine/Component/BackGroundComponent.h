#pragma once
#include <Component/RenderComponent.h>

using namespace Microsoft::WRL;
class BackGroundComponent : public RenderComponent
{
public:
	BackGroundComponent();
	~BackGroundComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void UpdateFrames(const float& deltaSeconds);
	void ReleaseFrames();

	void LoadData(const std::wstring& path, const int& fps, const std::wstring& extension = L"jpg", const int& quality = 95, const bool& bIsBackGround = false);
	
	void LoadFromFolder(const std::wstring& folderPath, int fps, const std::wstring& extension);
	
	void LoadFrame(size_t frameIndex);
	void LoadFrameFromFolder(size_t frameIndex);
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();
	std::vector<std::wstring> files;
	std::wstring fileDirPath; // ���� ���� ���
	std::vector<std::shared_ptr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage ������Ʈ
	const size_t cacheSize = 2; // ĳ���� ������ ��

	void Play() { bPlay = true; }
	void Stop() { bPlay = false; }

	void SetPlayer(gameObject* player) { m_player = player; }

	size_t m_curClip = 0;
	size_t m_maxClipSize = 0;
	bool bPlay = false;
	float m_fFPSTime = 1.0f / 18.0f;
	float m_fFPSLastTime = 0;

	gameObject* m_player = nullptr;
	bool bImage = false;
	std::wstring imageExtension;
};

