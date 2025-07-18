#pragma once
/*
* @brief Define ���ӽ����̽�.
* @details �� ���ӽ����̽��� ���� ������ ���� ��� �� ������ �����մϴ�.
*/

namespace Define
{
	static const int SCREEN_WIDTH = 1024;	// ȭ�� �ʺ�
	static const int SCREEN_HEIGHT = 768;	// ȭ�� ����
	static const int FRAME_RATE = 60;		// ������ ����Ʈ
	static const std::wstring WINDOW_CLASS_NAME = L"Sun, Earth, Moon";	// ������ Ŭ���� �̸�
	static const std::wstring WINDOW_TITLE = L"Sun, Earth, Moon Title";		// ������ Ÿ��Ʋ
	static const std::wstring MODULE_PATH;		// ��� ���
	static const std::wstring WORKING_PATH;	// �۾� ���丮 ���
	static const bool ENABLE_DEBUG_MODE = true; // ����� ��� Ȱ��ȭ ����

	static const std::wstring BASE_RESOURCE_PATH = L"Resource\\";
	static const std::wstring BASE_EXTENSION_PATH = L"Extension\\";
	static const float PI = 3.14159265358979323846264f;
	static const float GRAVITYCONSTANT = 9.8f;
	static const float MIN_OVERLAP_EPSILON = 0.001f; // float ���� ������ �ּ� ��ħ��

	enum class EDrawType
	{
		ScreenSpace,
		WorldSpace,
		Max
	};

	enum class ETextFormat
	{
		TopLeft,
		TopCenter,
		TopRight,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		BottomLeft,
		BottomCenter,
		BottomRight,
		Max
	};
	enum class ETransformType
	{
		D2D,
		Unity,
		Max
	};
	enum class ERenderLayer
	{
		VideoComponent,
		SpriteComponent,
		Animator,
		BoxComponent,
		TextRenderComponent,
		Max
	};

	// Unreal �� Unity �ֿ� Tick/Update ��
	// TG_PrePhysics       : EarlyUpdate         // (AI/�Է�, Unity�� EarlyUpdate)
	// TG_StartPhysics     : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate �غ�)
	// TG_DuringPhysics    : FixedUpdate         // (���� ����, Unity�� FixedUpdate)
	// TG_EndPhysics       : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate ��ó��)
	// TG_PostPhysics      : Update/LateUpdate   // (���ӷ���, Unity�� Update/LateUpdate)
	// TG_PostUpdateWork   : PreRender/Render    // (ī�޶�, ������, Unity�� PreRender/Render)
	// TG_LastDemotable    : (Ư�� ��Ȳ��, �𸮾��� �ܰ�)
	// TG_NewlySpawned     : (�� ��ü ó����, �𸮾��� �ܰ�)
	enum class ETickingGroup
	{
		TG_PrePhysics,       // Unity: EarlyUpdate (AI/�Է�)
		TG_StartPhysics,     // Unity: FixedUpdate ����
		TG_DuringPhysics,    // Unity: FixedUpdate (���� ����)
		TG_EndPhysics,       // Unity: FixedUpdate ����
		TG_PostPhysics,      // Unity: Update/LateUpdate (���ӷ���)
		TG_PostUpdateWork,   // Unity: PreRender/Render (ī�޶�, ������)
		TG_LastDemotable,
		TG_NewlySpawned,
		TG_MAX,
	};

	enum class EScriptGroup
	{
		SG_Awake,
		SG_OnStart,
		SG_OnEnd,
		SG_OnDestroy,
		SG_OnCollisionEnter2D,
		SG_OnCollisionStay2D,
		SG_OnCollisionExit2D,
		SG_OnTriggerEnter2D,
		SG_OnTriggerStay2D,
		SG_OnTriggerExit2D,
		SG_MAX,
	};

	enum class ParameterType { Int, Float, Bool, Trigger };

	enum class EBoxType { RenderDebugBox, ColliderDebugBox, Max };

	enum class ERigidBodyType { Dynamic, Kinematic, Static, Max };

	enum class ERigidBodyState { Space, Ground, OnRigidBody, Max};
};

#define VK_A 0x41  // 'A'
#define VK_B 0x42  // 'B'
#define VK_C 0x43  // 'C'
#define VK_D 0x44  // 'D'
#define VK_E 0x45  // 'E'
#define VK_F 0x46  // 'F'
#define VK_G 0x47  // 'G'
#define VK_H 0x48  // 'H'
#define VK_I 0x49  // 'I'
#define VK_J 0x4A  // 'J'
#define VK_K 0x4B  // 'K'
#define VK_L 0x4C  // 'L'
#define VK_M 0x4D  // 'M'
#define VK_N 0x4E  // 'N'
#define VK_O 0x4F  // 'O'
#define VK_P 0x50  // 'P'
#define VK_Q 0x51  // 'Q'
#define VK_R 0x52  // 'R'
#define VK_S 0x53  // 'S'
#define VK_T 0x54  // 'T'
#define VK_U 0x55  // 'U'
#define VK_V 0x56  // 'V'
#define VK_W 0x57  // 'W'
#define VK_X 0x58  // 'X'
#define VK_Y 0x59  // 'Y'
#define VK_Z 0x5A  // 'Z'
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39 

#define VK_OEM_4 0xDB  // [
#define VK_OEM_6 0xDD  // ]