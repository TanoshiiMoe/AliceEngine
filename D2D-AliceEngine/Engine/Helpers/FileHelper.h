#pragma once
class FileHelper
{
	/*
	* 파일 관련입니다. Resource 파일을 복사합니다.
	*/
public:

	static bool CreateDirectoryIfNotExists(const std::wstring& dir) {
		if (CreateDirectoryW(dir.c_str(), NULL) ||
			GetLastError() == ERROR_ALREADY_EXISTS) {
			return true;
		}
		return false;
	}

	static void ClearOutputDirectory(const std::wstring& dir) {
		WIN32_FIND_DATAW findFileData;
		std::wstring searchPath = dir + L"\\*";
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) return;

		do {
			std::wstring fileName = findFileData.cFileName;
			if (fileName == L"." || fileName == L"..") continue;

			std::wstring filePath = dir + L"\\" + fileName;
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// 재귀적으로 하위 폴더 삭제 (필요하다면)
				// RemoveDirectoryW(filePath.c_str());
			}
			else {
				DeleteFileW(filePath.c_str());
			}
		} while (FindNextFileW(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	static std::vector<std::wstring> GetFilesWithPattern(const std::wstring& outputDir, const std::wstring& pattern) {
		std::vector<std::wstring> files;
		std::wstring searchPath = outputDir + L"\\" + pattern;
		WIN32_FIND_DATAW findFileData;
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::wstring filePath = outputDir + L"\\" + findFileData.cFileName;
					files.push_back(filePath);
				}
			} while (FindNextFileW(hFind, &findFileData) != 0);
			FindClose(hFind);
		}
		std::sort(files.begin(), files.end());
		return files;
	}

	static std::wstring GetProjectRootPath()
	{
		WCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, buffer, MAX_PATH);
		std::wstring exePath(buffer);
		size_t lastSlash = exePath.find_last_of(L"\\/");
		if (lastSlash != std::wstring::npos)
			return exePath.substr(0, lastSlash); // 실행 파일이 있는 폴더
		return L"";
	}

	static std::wstring ToAbsolutePath(const std::wstring& baseDir)
	{
		// 이미 절대경로라면 그대로 반환
		if (baseDir.size() > 1 && (baseDir[1] == L':' || baseDir[0] == L'\\' || baseDir[0] == L'/'))
			return baseDir;

		std::wstring root = GetProjectRootPath();
		if (root.empty()) return baseDir;
		return root + L"\\" + baseDir;
	}

	static bool CreateDirectoryRecursive(const std::wstring& path) {
		if (path.empty()) return false;

		DWORD attr = GetFileAttributesW(path.c_str());
		if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
			// 이미 폴더 존재
			return true;
		}

		size_t pos = path.find_last_of(L"\\/");
		if (pos != std::wstring::npos) {
			std::wstring parent = path.substr(0, pos);
			if (GetFileAttributesW(parent.c_str()) == INVALID_FILE_ATTRIBUTES) {
				if (!CreateDirectoryRecursive(parent))
					return false;
			}
		}

		return CreateDirectoryW(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}

	// 파일 복사 함수 (덮어쓰기)
	static bool CopyFileOverwrite(const std::wstring& src, const std::wstring& dst) {
		return CopyFileW(src.c_str(), dst.c_str(), FALSE) != 0;
	}

	// 디렉토리 재귀 복사 함수
	static bool CopyDirectoryRecursive(const std::wstring& sourceDir, const std::wstring& targetDir) {
		WIN32_FIND_DATAW findData;
		std::wstring searchPath = sourceDir + L"\\*";
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) {
			// 소스 폴더가 없거나 비어있음
			return false;
		}

		// 타겟 폴더 생성
		if (!CreateDirectoryRecursive(targetDir)) {
			FindClose(hFind);
			return false;
		}

		do {
			const std::wstring fileName = findData.cFileName;

			// "." 및 ".." 제외
			if (fileName == L"." || fileName == L"..") continue;

			std::wstring sourcePath = sourceDir + L"\\" + fileName;
			std::wstring targetPath = targetDir + L"\\" + fileName;

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// 하위 폴더 재귀 복사
				if (!CopyDirectoryRecursive(sourcePath, targetPath)) {
					FindClose(hFind);
					return false;
				}
			}
			else {
				// 파일 복사
				if (!CopyFileOverwrite(sourcePath, targetPath)) {
					FindClose(hFind);
					return false;
				}
			}
		} while (FindNextFileW(hFind, &findData) != 0);

		FindClose(hFind);
		return true;
	}

	static void CopyFilesToBuildPath(const std::wstring& _str)
	{
		std::wstring sourcePath = ToAbsolutePath(L"") + L"..\\..\\" + _str;
		std::wstring targetPath = ToAbsolutePath(L"") + L"\\" + _str;

		CreateDirectoryIfNotExists(targetPath);

		bool result = CopyDirectoryRecursive(sourcePath, targetPath);
		assert(result && L"Resource 복사 실패");
	}


	// 재귀적으로 파일 경로를 수집하는 함수
	static void CollectFilePathsRecursive(const std::wstring& directory, std::vector<std::wstring>& outFiles)
	{
		WIN32_FIND_DATAW findData;
		std::wstring searchPath = directory + L"\\*";
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) {
			// 폴더가 없거나 비어있음
			return;
		}

		do {
			const std::wstring fileName = findData.cFileName;
			if (fileName == L"." || fileName == L"..") continue;

			std::wstring fullPath = directory + L"\\" + fileName;

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// 하위 폴더 재귀 탐색
				CollectFilePathsRecursive(fullPath, outFiles);
			}
			else {
				// 파일 경로 저장
				outFiles.push_back(fullPath);
			}
		} while (FindNextFileW(hFind, &findData) != 0);

		FindClose(hFind);
	}

	static void ResourceFilesInBuildPath(const std::wstring& _str, std::vector<std::wstring>& filesPaths)
	{
		std::wstring path = ToAbsolutePath(L"") + _str;
		CollectFilePathsRecursive(path, filesPaths);
		//assert(result && L"Resource 탐색 실패");
	}

	// 파일 이름과 확장자를 추출하는 함수
	// 반환값: {파일이름, 확장자}
	static std::pair<std::wstring, std::wstring> ExtractFileNameAndExtension(const std::wstring& absPath)
	{
		// 마지막 경로 구분자 위치
		size_t lastSlash = absPath.find_last_of(L"\\/");
		std::wstring fileNameWithExt = (lastSlash == std::wstring::npos) ? absPath : absPath.substr(lastSlash + 1);

		// 마지막 점(.) 위치
		size_t lastDot = fileNameWithExt.find_last_of(L'.');
		if (lastDot == std::wstring::npos) {
			// 확장자가 없는 경우
			return { fileNameWithExt, L"" };
		}

		std::wstring fileName = fileNameWithExt.substr(0, lastDot);
		std::wstring extension = fileNameWithExt.substr(lastDot + 1);
		return { fileName, extension };
	}

	// 해당 경로의 폴더와 이름을 분리하는 함수
	static std::pair<std::wstring, std::wstring> ExtractFileDirectoryAndName(const std::wstring& absPath)
	{
		std::wstring inputPath = absPath;
		size_t lastSlash = inputPath.find_last_of(L"/\\");
		size_t lastDot = inputPath.find_last_of(L'.');

		std::wstring folderPath = inputPath.substr(0, lastSlash);
		std::wstring fileName = inputPath.substr(
			lastSlash + 1,
			(lastDot != std::wstring::npos ? lastDot : inputPath.length()) - (lastSlash + 1)
		);

		return { folderPath, fileName };
	}


	// 파일 경로에서 해당 폴더만 가져오는 함수 (std::wstring 버전)
	// "\\Resource\\content\\video\\aru.mp4" -> "\\Resource\\content\\video"
	static std::wstring get_folder_path(const std::wstring& filepath) {
		std::size_t last_slash = filepath.find_last_of(L"\\/");
		if (last_slash == std::wstring::npos)
			return filepath; // 슬래시가 없으면 전체가 폴더로 간주
		return filepath.substr(0, last_slash);
	}
};

