#pragma once
#include "pch.h"
#include "FileHelper.h"
#include <Manager/PackageResourceManager.h>
#include <manager/D2DRenderManager.h>

class FFmpegHelper
{
public:
	static std::vector<std::wstring> GetFramesFromVideo(const std::wstring& baseDir, const int& fps, const std::wstring& extension, const float& quality, bool bIsBackGround) {
		// ��� ���� ��θ� �迭�� ����
		std::vector<std::wstring> frameFiles;
		WCHAR folderNameBuffer[100] = L"frames";
		WCHAR prefixBuffer[100] = L"frame";

		std::wstring inputPath = FileHelper::ToAbsolutePath(baseDir);
		//std::wstring inputPath =  L"../" + Define::BASE_RESOURCE_PATH + baseDir; // ��: L"BackGround/Yuuka.mp4"

		// ���ϸ�(Ȯ���� ����) ����
		size_t lastSlash = inputPath.find_last_of(L"/\\");
		size_t lastDot = inputPath.find_last_of(L'.');
		std::wstring fileName = inputPath.substr(
			lastSlash + 1,
			(lastDot != std::wstring::npos ? lastDot : inputPath.length()) - (lastSlash + 1)
		);

		// ���� ���� ��� ����
		std::wstring folderPath = inputPath.substr(0, lastSlash);

		// ���� ��� ���� ��� ����
		std::wstring parentDir = folderPath + L"\\" + fileName;
		std::wstring outputDir = parentDir + L"\\" + folderNameBuffer;
		// ��: "BackGround\\Yuuka\\frames"

		FileHelper::CreateDirectoryIfNotExists(parentDir);
		if (FileHelper::CreateDirectoryIfNotExists(parentDir))
		{
			if (FileHelper::CreateDirectoryIfNotExists(outputDir))
			{
				std::vector<std::wstring> temp = FileHelper::GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*." + extension));
				if (temp.empty())
					FileHelper::ClearOutputDirectory(outputDir);
				else
					return temp;
			}
		}

		std::wstring options = L"";
		if (extension == L"png") 
		{
			options = L"-compression_level " + std::to_wstring(int(quality));
		}
		else 
		{
			double qv = 31.0 - (quality / 100.0) * 30.0;
			int mappedQuality = static_cast<int>(qv);
			if (mappedQuality < 1) mappedQuality = 1;
			if (mappedQuality > 31) mappedQuality = 31;
			options = L"-q:v " + std::to_wstring(mappedQuality);
		}

		std::wstring ffmpegPath = Define::BASE_EXTENSION_PATH + L"FFmpeg\\ffmpeg.exe";
		std::wstring vfOption;
		if (bIsBackGround) {
			vfOption = L"fps=" + std::to_wstring(fps) + L",scale=" + std::to_wstring(D2DRenderManager::GetInstance().GetApplicationSize().x) + L":" + std::to_wstring(D2DRenderManager::GetInstance().GetApplicationSize().y);
		}
		else {
			vfOption = L"fps=" + std::to_wstring(fps);
			// scale �ɼ� ����
		}

		std::wstring command = ffmpegPath + L" -y -i \"" + inputPath + L"\" -vf " + vfOption + L" " +
			options + L" \"" + outputDir + L"\\" + prefixBuffer + L"_%04d." + extension + L"\"";

		STARTUPINFOW si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		// ffmpeg ����
		if (!CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) 
		{
			assert(L"ffmpeg ���� ����\n\n��ο� ffmpeg.exe�� �ִ��� Ȯ���ϼ���.");
			return std::vector<std::wstring>();
		}
		// ffmpeg ���μ����� ���� ������ ���
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		PackageResourceManager::GetInstance().Initialize();

		return FileHelper::GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*."+ extension));
	}
};

