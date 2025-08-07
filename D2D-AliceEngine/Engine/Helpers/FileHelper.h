#pragma once
class FileHelper
{
	/*
	* ���� �����Դϴ�. Resource ������ �����մϴ�.
	*/
public:

	static bool CreateDirectoryIfNotExists(const std::wstring& dir);

	static void ClearOutputDirectory(const std::wstring& dir);

	static std::vector<std::wstring> GetFilesWithPattern(const std::wstring& outputDir, const std::wstring& pattern);

	static std::wstring GetProjectRootPath();
	static std::wstring ToAbsolutePath(const std::wstring& baseDir);

	static bool CreateDirectoryRecursive(const std::wstring& path);
	// ���� ���� �Լ� (�����)
	static bool CopyFileOverwrite(const std::wstring& src, const std::wstring& dst);
	// ���丮 ��� ���� �Լ�
	static bool CopyDirectoryRecursive(const std::wstring& sourceDir, const std::wstring& targetDir);

	static void CopyFilesToBuildPath(const std::wstring& _str);

	// ��������� ���� ��θ� �����ϴ� �Լ�
	static void CollectFilePathsRecursive(const std::wstring& directory, std::vector<std::wstring>& outFiles);
	static void ResourceFilesInBuildPath(const std::wstring& _str, std::vector<std::wstring>& filesPaths);
	// ���� �̸��� Ȯ���ڸ� �����ϴ� �Լ�
	// ��ȯ��: {�����̸�, Ȯ����}
	static std::pair<std::wstring, std::wstring> ExtractFileNameAndExtension(const std::wstring& absPath);
	// �ش� ����� ������ �̸��� �и��ϴ� �Լ�
	static std::pair<std::wstring, std::wstring> ExtractFileDirectoryAndName(const std::wstring& absPath);
	// ���� ��ο��� �ش� ������ �������� �Լ� (std::wstring ����)
	// "\\Resource\\content\\video\\aru.mp4" -> "\\Resource\\content\\video"
	static std::wstring get_folder_path(const std::wstring& filepath);

	static std::vector<BYTE> LoadBinaryFile(const std::wstring& filePath);
};

