#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <unordered_map>

/*
*  @brief : 패키지 리소스 관리 클래스
 * @details : Direct2D 비트맵 생성 및 관리
 */

enum class EResourceSpecifier
{
	Bitmap,
	Max
};

struct FPacakageInfo
{
	EResourceSpecifier resourceType;
	std::wstring name; // 리소스 이름
	std::wstring fileExtension; // 파일 확장자
	std::wstring uuid; // 고유 식별자
	bool bIsLoaded = false; // 로드 여부
	FPacakageInfo()
		: resourceType(EResourceSpecifier::Max), name(L""), fileExtension(L""), uuid(L""), bIsLoaded(false)
	{
	}
	FPacakageInfo(const EResourceSpecifier& _resourceType, const std::wstring& name, const std::wstring& extension, const std::wstring& uuid)
		: resourceType(_resourceType), name(name), fileExtension(extension), uuid(uuid), bIsLoaded(false)
	{
	}
};

struct FMemoryInfo
{
	std::wstring VRAMUssage;
	std::wstring DRAMUssage;
	std::wstring PageFile;
	FMemoryInfo() : VRAMUssage(L""), DRAMUssage(L""), PageFile(L"") {}
};

class PackageResourceManager : public Singleton<PackageResourceManager>
{
public:
	PackageResourceManager();
	~PackageResourceManager();

	void Initialize();
	// For ImageDraw
	ComPtr<IWICImagingFactory> m_wicImagingFactory;

	std::weak_ptr<ID2D1Bitmap1> CreateBitmapFromFile(const wchar_t* path);

	std::wstring FormAtBytes(UINT64 bytes);
	FMemoryInfo GetMemoryInfo();

	/*
	* key 값으로는 Resource에 대한 절대 경로를 사용합니다.
	*/
	 std::unordered_map<std::wstring, FPacakageInfo> m_preloadedPaths; // 미리 로드된 비트맵 캐시

	 // key 값은 FPacakageInfo에 있는 uuid 입니다.
	 std::unordered_map<std::wstring, std::shared_ptr<ID2D1Bitmap1>> m_loadedBitmaps; // 로드된 비트맵 캐시
};