#pragma once
#include "pch.h"
#include "Application.h"
#include "D2DRenderer.h"

void Application::Initialize()
{
	m_pD2DRenderer = new D2DRenderer();

	char szPath[MAX_PATH] = { 0, };
	GetModuleFileNameA(NULL, szPath, MAX_PATH); // 현재 모듈의 경로
	m_ModulePath = szPath; // 모듈 경로
	GetCurrentDirectoryA(MAX_PATH, szPath); // 현재 작업 디렉토리
	m_WorkingPath = szPath; // 작업 디렉토리
	OutputDebugStringA(std::string(std::string(m_ModulePath) + std::string("\n")).c_str());
	OutputDebugStringA(std::string(std::string(m_WorkingPath) + std::string("\n")).c_str());

	m_hInstance = GetModuleHandle(NULL); // 현재 프로세스의 핸들
}

void Application::Run()
{

}

void Application::Uninitialize()
{
	m_pD2DRenderer->Uninitialize();
	m_pD2DRenderer = nullptr;
}