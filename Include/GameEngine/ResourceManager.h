#pragma once
#include "IManager.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "FileManager.h"

// 일어나서 그래픽스 엔진 리소스매니저에 연결하고 테스트 해볼 것

class SoundManager;
class FileManager;

/// <summary>
/// 게임 엔진의 리소스들을 관리하는 클래스입니다.
/// 2024.9.16. [Riun] 매니저를 제작합니다.
/// 구현에 대한 자세한 슈도 코드와 스터디 내용 : https://www.notion.so/Resource-Manager-Game-Engine-1033e506dcbd804d8457eb51c46cafb8?pvs=4
/// </summary>
class ResourceManager
	: public IManager
{
public:
	ResourceManager(std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine, SoundManager* _pSoundManager);
	~ResourceManager() = default;

	virtual bool Initialize() override;/* { return true; }*/
	virtual void FixedUpdate(float _fixedDTime) override{}
	virtual void Update(float _dTime) override{}
	virtual void LateUpdate(float _dTime) override{}
	virtual void Finalize() override;

	// 그래픽스 엔진에 해당되는 리소스 추가-------------------------------
	/// <summary> 
	/// 해당되는 파일 1개를 추가한다. 
	/// </summary>
	/// <param name="_fileName">파일 이름</param>
	void AddResource(const std::string& _fileName);

	/// <summary> 
	/// 특정 디렉토리의 파일들을 추가한다. 
	/// </summary>
	/// <param name="_fileName">디렉토리 이름</param>
	void AddFilesInDir(const std::string& _dirName);

	// 도저히 귀찮아서 못하겠다. 그냥 임시로 추가한다.
	void AddFilesInDir2D(const std::string& _dirName);
	void AddFilesInDir3D(const std::string& _dirName);
	void AddFilesInDirModel(const std::string& _dirName);
	void AddFilesInDirAni(const std::string& _dirName);
	void AddFilesInDirDDS(const std::string& _dirName);
	void AddFilesInDirBGMSound(const std::string& _dirName);
	void AddFilesInDirSFXSound(const std::string& _dirName);

	/// <summary> 
	/// 특정 디렉토리의 모든 하위 파일들을 전부 추가한다. 
	/// </summary>
	/// <param name="_fileName">디렉토리 이름</param>
	void AddFilesInDirTree(const std::string& _dirName);

	/// <summary> 
	/// 폰트는 별개로 추가한다. 
	/// </summary>
	/// <param name="_fileName">파일 이름</param> 
	/// <param name="_size">사이즈</param> 
	/// <param name="_isKorean">true : 한국어, false : 영어만</param>
	void AddFont(const std::string& _fileName, float _size, bool _isKorean);

	/// <summary> 폰트를 모두 추가했으면 셋팅 완료를 호출함 </summary>
	void FontSetFinish();

	// 그래픽스 엔진에서 리소스를 가져온다. ------------------------------
	
	/// <summary> 
	/// 모델의 정보를 가지고 온다. 
	/// </summary>
	/// <param name="_modelName">모델 이름</param> 
	/// <returns>Model*</returns>
	Model* Get_ModelInfo(std::string _modelName);

	/// <summary> 
	/// 애니메이션 정보를 가져온다. 
	/// </summary>
	/// <param name="_animeName">애니메이션 이름</param> 
	/// <returns>Animation*</returns>
	Animation* Get_Animation(std::string _animeName);

	/// <summary> 
	/// 텍스처를 가지고 온다. 
	/// </summary>
	/// <param name="_textureName">텍스처 이름</param> 
	/// <returns> ComPtr&lt;ID3D11ShaderResourceView&gt;</returns>
	ComPtr<ID3D11ShaderResourceView> Get_Textures(std::string _textureName);

	/// <summary> 
	/// 폰트를 가지고 온다. 
	/// </summary>
	/// <param name="_fontName">폰트 이름</param> <returns>ImFont*</returns>
	ImFont* Get_Font(std::string _fontName);

	/// <summary> 
	/// 모델의 특정 본을 가져온다 -> 장비에 적용할 때 사용한다. 
	/// </summary>
	/// <param name="_modelName">모델 이름</param> 
	/// <param name="_boneName">본 이름</param> 
	/// <returns>본의 인덱스 </returns>
	int Get_TargetModelBoneIndex(std::string _meshName, std::string _boneName);

	// 무기의 렌더시 쓰는 함수 -> 본의 offsetMatrix가 곱해진 Matrix를 받아온다.
	Matrix GetTargetBoneMatrix(std::string _targetModelName, std::string _targetBoneName);

	/// <summary>
	/// 실제 본의 위치를 받아오는 함수 -> 본의 offsetMatrix가 곱해지지 않은 본의 위치값만 받아온다 
	/// </summary>
	/// <param name="_targetModel">모델 이름</param> 
	/// <param name="_targetBoneName">본 이름</param> 
	/// <param name="_scale">스케일 = 1.0f</param>
	/// <returns>실제 본의 위치 매트릭스</returns>
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale = 1.0f);

	/// <summary> 
	/// 충돌 체크를 위해 AABB를 가져오는 함수 
	/// </summary>
	/// <param name="__targetModel">모델 이름</param> 
	/// <returns>AABB</returns>
	AABB Get_AABB(std::string __targetModel);

	// 그래픽스 엔진에서 리소스를 삭제한다.(폰트 포함 삭제됨) --------------
	/// <summary>
	///  해당되는 파일 1개를 삭제한다. 
	/// </summary>
	/// <param name="_fileName">파일 이름</param>
	void RemoveResource(const std::string& _fileName);

	/// <summary>
	///  특정 디렉토리의 파일들을 삭제한다. 
	/// </summary>
	/// <param name="_fileName">디렉토리 이름</param>
	void RemoveFilesInDir(const std::string& _dirName);

	/// <summary>
	///  특정 디렉토리의 모든 하위 파일들을 전부 삭제한다. 
	/// </summary>
	/// <param name="_fileName">디렉토리 이름</param>
	void RemoveFilesInDirTree(const std::string& _dirName);
	
	// Csv 파서 -------------------------------------------------------
	/// <summary> 
	/// 첫 줄을 제외하고 읽는 string 값이 포함된 cvs 문서를 읽을 경우 쓰는 파서 
	/// </summary>
	/// <typeparam name="T">테이블 Info</typeparam> 
	/// <param name="_fileName">파일 이름</param> 
	/// <returns>std::vector<T></returns>
	template <typename T>
	std::vector<T> ParseCSV(const std::string& _fileName);

	// 정수값만 들어갈 경우 처리하는 게 다르기 때문에 템플릿으로 만들지 않았다.
	/// <summary> 
	/// 첫 줄을 포함하고 읽는 숫자값(정수)만 포함된 cvs 문서를 읽을 경우 쓰는 파서 
	/// </summary>
	/// <param name="_fileName">파일 이름</param> 
	/// <returns>std::vector<std::vector<int>></returns>
	std::vector<std::vector<int>> ParseMapCSV(const std::string& _fileName);

	// 파일 매니저 -----------------------------------------------------
	/// <summary>
	/// 파일 매니저를 가져온다. 
	/// </summary>
	FileManager* GetFileManager();
private:
	// 멤버 함수 포인터를 사용하기 위해 using 선언
	using AddFunctionType = void(DearsGraphicsEngine::*)(std::string, std::string);	
	using RemoveFuctionType = void(DearsGraphicsEngine::*)(std::string);

	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// 그래픽스 엔진 객체
	FileManager* mpFileManager;								// 파일 매니저
	SoundManager* mpSoundManager;

	AddFunctionType FunctionType(const std::string& _basePath);			// 추가함수
	RemoveFuctionType EraseFunctionType(const std::string& _basePath);	// 삭제함수
	bool mIsAlreadyFontLoaded; // 이미 폰트가 추가됐는지 확인해서, 추가 완료를 막는 플래그
};

template <typename T>
std::vector<T> ResourceManager::ParseCSV(const std::string& _fileName)
{
	std::vector<T> units = {};
	std::string filepath = mpFileManager->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// 파일 오픈 실패 시 빈 벡터 반환
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return units;
	}

	int columnCount = 0;
	// 첫 번째 줄(헤더)을 읽어들임 -> 칼럼 개수 카운트
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string column;

		// 쉼표로 구분된 각 필드를 세어 칼럼 개수 카운트
		while (std::getline(ss, column, ','))
		{
			columnCount++;
		}
	}

	// 두 번째 줄부터 읽어준다.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;
		while (std::getline(ss, cell, ','))
		{
			row.push_back(cell);
		}
		if (row.size() >= columnCount)
		{  // 모든 칼럼이 있는지 확인
			units.emplace_back(row);
		}
		else
		{
			std::cerr << "Incomplete row found and skipped." << std::endl;
		}
	}
	return units;
}