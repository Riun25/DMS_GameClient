#include "ClassManager.h"
#include "BoneGroupComponent.h"
#include "EquipmentComponent.h"
#include "Texture3D.h"

ClassManager::ClassManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager)
{
	mpClassMeshInfoVec = {};
	mpResourceManager = _pResourceManager;
}

ClassManager::~ClassManager()
{
}

void ClassManager::Initialize()
{
	mStringToClassTypeVec = {
	{"Mercenary", ClassType::MERCENARY},
	{"Archer", ClassType::ARCHER},
	{"Maid", ClassType::MAID},
	{"Monk", ClassType::MONK},
	{"Ninja", ClassType::NINJA},
	{"Viking", ClassType::VIKING},
	{"Wizard", ClassType::WIZARD},
	{"RVC_0", ClassType::RVC_0},
	{"RVC_R1", ClassType::RVC_R1},
	{"RVC_C", ClassType::RVC_C} };

	// 여기서 정보를 받아와서 저장하게 할 것임
	mpClassMeshInfoVec = mpResourceManager->ParseCSV<CharacterMeshInfo>("CharacterMesh.csv");
}

void ClassManager::AddUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (const auto& unitMeshInfo : mpClassMeshInfoVec)
	{
		if (unitMeshInfo.className == _classType)
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				_entity->GetComponent<PlayerComponent>().mClassType = ConvertStringToClasstype(_classType);
			}
			else if (_entity->HasComponent<EnemyComponent>())
			{
				_entity->GetComponent<EnemyComponent>().mClassType = ConvertStringToClasstype(_classType);
			}
			_entity->AddComponent<MeshRenderer>(unitMeshInfo.fbxName, unitMeshInfo.meshName, true, true);
			auto& unitComp = _entity->AddComponent<AnimationComponent>();
			unitComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
			unitComp.mIsLoop = true;
			_entity->AddComponent<BoneGroupComponent>();
			_entity->AddComponent<Texture3D>(unitMeshInfo.textureName);
			_entity->AddComponent<EquipmentComponent>();
			return;
		}
	}
}

void ClassManager::UpdateUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (const auto& unitMeshInfo : mpClassMeshInfoVec)
	{
		if (unitMeshInfo.className == _classType)
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				_entity->GetComponent<PlayerComponent>().mClassType = ConvertStringToClasstype(_classType);
			}
			else if (_entity->HasComponent<EnemyComponent>())
			{
				_entity->GetComponent<EnemyComponent>().mClassType = ConvertStringToClasstype(_classType);
			}
			_entity->GetComponent<MeshRenderer>().m_file = unitMeshInfo.fbxName;
			_entity->GetComponent<MeshRenderer>().m_meshName = unitMeshInfo.meshName;
			_entity->GetComponent<Texture3D>().m_diffuse = unitMeshInfo.textureName;
			return;
		}
	}
}

std::vector<std::pair<std::string, ClassType>>* ClassManager::GetClassTypeVec()
{
	return &mStringToClassTypeVec;
}

ClassType ClassManager::ConvertStringToClasstype(const std::string& _classType)
{
	for (const auto& e : mStringToClassTypeVec)
	{
		if (e.first == _classType)
		{
			return e.second;
		}
	}

	return ClassType::LAST;// 매핑되지 않은 경우
}

std::string ClassManager::ConvertClasstypeToString(ClassType _classTypeEnum)
{
	for (const auto& e : mStringToClassTypeVec)
	{
		if (e.second == _classTypeEnum)
		{
			return e.first;
		}
	}
	return std::string();
}
