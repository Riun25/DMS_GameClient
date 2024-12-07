#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <unordered_set>
#include <list>
#include <unordered_map>

/// <summary>
/// 전반적으로 트리구조를 사용하도록 바꿨다.
/// 2024.9.15.
/// </summary>
class FileManager
{
public:
	FileManager();
	~FileManager();

private:

	struct DirData
	{
		std::string dirName;					// 디렉토리 이름(= 폴더 이름)
		std::list<std::string> pInDirFiles;		// 디렉토리 내 파일 리스트
		std::list<DirData*> subDirectories;		// 하위 디렉토리
	};

public:
	/// <summary> 초기화 : 리소스의 루트 함수를 정하고, 디렉토리 및 파일 구조를 트리에 저장한다. </summary>
	void Initialize();
	void Finalize();

public:
	/// <summary> 루트 DirData를 가져온다. </summary>
	/// <returns>DirData*</returns>
	DirData* GetRootDirData();

	/// <summary> 디렉토리 이름을 통해서 DirData 구조를 가져온다. </summary>
	/// <param name="_dirName">디렉토리 이름</param> <returns>DirData*</returns>
	DirData* GetDirData(const std::string& _dirName);

	/// <summary> 파일 이름을 통해서 DirData 구조를 가져온다. </summary>
	/// <param name="_fileName">파일 이름</param> <returns>DirData*</returns>
	DirData* GetDirDataFromFile(const std::string& _fileName);

	/// <summary> 파일 이름을 통해서 DirData 이름 가져온다. </summary>
	/// <param name="_fileName">파일 이름</param> <returns>std::string</returns>
	std::string GetDirName(const std::string& _fileName);

	/// <summary> 해당 파일의 BasePath를 가져온다. </summary>
	/// <param name="_fileName">파일 이름</param> <returns>std::string</returns>
	std::string GetBasePath(const std::string& _fileName);

	/// <summary> 모든 파일의 전체 경로를 가진 리스트를 가져온다. </summary>
	/// <returns> std::list&lt;std::string&gt; </returns>
	std::list<std::string> GetAllFilePathList();

	/// <summary> 현재 파일에 존재하는 디렉토리의 이름만을 출력한다. </summary>
	/// <param name="_dirData">디렉토리 데이터의 구조체</param> <returns>std::list&lt;std::string&gt;</returns>
	std::list<std::string> GetOnlySubDirNameList(DirData* _dirData);

	/// <summary> 현재 디렉토리에 존재하는 파일의 이름만을 출력한다. </summary>
	/// <param name="_dirData">디렉토리 데이터의 구조체</param> <param name="_isOnlyNowDir">true : 해당 디렉토리에 포함된 파일 이름들만 가져옴, false : 해당 디렉토리부터 모든 하위까지 파일 이름들을 가져옴</param>
	/// <returns>std::list&lt;std::string&gt;</returns>
	std::list<std::string> GetAllFilesInDirList(DirData* _dirData, bool _isOnlyNowDir = true);

	/// <summary> 파일 구조를 출력한다. </summary>
	/// <param name="mIsOnlyDir">true : 디렉토리만 출력, false : 디렉토리와 파일 전체 출력</param>
	void PrintAll(bool mIsOnlyDir);

	/// <summary> 파일의 전체 경로를 출력한다. </summary>
	/// <param name="_fileName">파일 이름</param> <returns>std::string</returns>
	std::string GetFullFilePath(const std::string& _fileName);

private:
	DirData* mRootDirData = nullptr;			// 최상위 디렉토리
	std::filesystem::path mResourceBaseDir;		// 리소스 위치

private:
	// 파일의 전체적인 구조 및 디렉토리 데이터 트리에 저장한다.
	void SaveFolderStructure(const std::filesystem::path& _directory, DirData* _currentDir);
	// 파일 처리
	std::string ProcessFilePath(const std::filesystem::path& _filePath);

	/// 최적화를 위하여 재귀함수만 따로 떼어서 제작했다. (리스트의 중복 생성 방지)
	// 전체 경로를 찾는 재귀함수
	void SearchAllFilePaths(DirData* _currentDir, const std::string& _parentPath, std::list<std::string>& _filePathList);
	// 디렉토리의 이름 통해 DirData* 형식을 찾는 재귀함수
	DirData* SearchDirName(DirData* _currentDir, const std::string& _DirName);
	// 파일의 이름 통해 DirData* 형식을 찾는 재귀함수
	DirData* SearchDirDataFromFile(DirData* _currentDir, const std::string& _fileName);
	// 특정 파일 내에 존재하는 모든 파일을 찾는 재귀함수
	void SearchAllFilesInDir(DirData* _dirData, std::list<std::string>& _fileNameList);

	// BasePath의 경로를 뽑아낸다. 폴더 이름만 입력하면 알아서 \\까지 출력함..
	std::string SearchBasePath(DirData* currentDir, const std::string& fileName, const std::string& parentPath);

	// 디렉토리 전체 구조 출력
	void PrintDirStructure(DirData* _currentDir, int _depth = 0);
	// 디렉토리와 그 안에 포함된 폴더 모두를 출력
	void PrintDirNFileStructure(DirData* _currentDir, int _depth = 0);
};

// 2024.6.4.
// 파일 시스템의 디렉토리 구조는 트리 구조와 매우 유사하다.
// 이 때문에 파일 시스템의 디렉토리 구조를 활용하여 트리 구조를 간접적으로 사용하는 것이 가능하다.
// 따로 unordered_map을 사용해서 매칭시킬 이유가 그다지 없다. 그냥 디렉터리만 찾는 함수를 이용해 이름을 매칭시키는 건 크게 낭비 없음

// 자료구조로 그냥 리스트를 쓴 이유 : 데이터 개수가 우리가 많아봤자 대부분 100개 이하라고 판단했다.

// 2024.9.15.
// <filesystem>의 구조와 흡사하게 트리의 구조를 짜서, 초반에 모든 정보를 트리 구조로 저장하게끔 바꿨다.
// 초반에 이 라이브러리에서 가져와서 내 트리 구조에 저장하고, 이후의 모든 것은 저장된 트리구조를 통해 탐색한다.
// 모든 파일을 탐색하지 않고 폴더만 검색할 수 있게 되었다.