#pragma once
#include "pch.h"

namespace Define
{
	static const int SCREEN_WIDTH = 1024;	// 화면 너비
	static const int SCREEN_HEIGHT = 768;	// 화면 높이
	static const int FRAME_RATE = 60;		// 프레임 레이트
	static const wchar_t* WINDOW_CLASS_NAME;	// 윈도우 클래스 이름
	static const wchar_t* WINDOW_TITLE;		// 윈도우 타이틀
	static const std::string MODULE_PATH;		// 모듈 경로
	static const std::string WORKING_PATH;	// 작업 디렉토리 경로
	static const bool ENABLE_DEBUG_MODE = true; // 디버그 모드 활성화 여부

	static const std::wstring BASE_RESOURCE_PATH = L"../Resource/";
	static const float PI = 3.14159265f;
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