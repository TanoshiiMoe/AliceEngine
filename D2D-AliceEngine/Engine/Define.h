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
};

