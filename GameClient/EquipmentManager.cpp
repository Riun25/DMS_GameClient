#include "EquipmentManager.h"
#include "Transform.h"
#include "Texture3D.h"
#include "TargetBoneComponent.h"
#include "BoneGroupComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "WeaponComponent.h"
#include "BoxCollider.h"
#include "Entity.h"
#include "EquipmentComponent.h"
#include "NPCComponent.h"

EquipmentManager::EquipmentManager(entt::registry& _registry, EntityManager* _pEntityManager
	, PhysicsManager* _pPhysicsManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager)
	:mRegistry(_registry)
{
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpEquipmentMeshVec = {};
	mpWeaponMeshVec = {};
	mpRenderManager = _pRenderManager;
	mpResourceManager = _pResourceManager;
}

EquipmentManager::~EquipmentManager()
{
}

void EquipmentManager::Initialize()
{
	mEquipPart =
	{ {
	{0, "RigLPalm"},       // 0: L Hand
	{1, "RigLForearm"},    // 1: L Forearm
	{2, "RigLCollarbone"},   // 2: L Upperarm
	{3, "RigHead"},       // 3: Head1
	{4, "RigHead"},       // 4: Head2
	{5, "RigHead"},       // 5: Head3
	{6, "RigHead"},       // 6: Head4
	{7, "RigHead"},       // 7: Head5
	{8, "RigRPalm"},       // 8: R Hand
	{9, "RigRForearm"},    // 9: R Forearm
	{10, "RigRUpperarm"},  // 10: R Upperarm
	{11, "RigRibcage"},     // 11: Chest1
	{12, "RigRibcage"},     // 12: Chest2
	{13, "RigPelvis"}      // 13: Pelvis
} };

	//mpEquipmentMeshVec = mpResourceManager->ParseCSV<EquipmentMeshInfo>("EquipmentNum_ver1.csv");
	mpEquipmentMeshVec = ParseEmptyCSV<EquipmentMeshInfo>("EquipmentNum_ver2.csv");
	mpWeaponMeshVec = mpResourceManager->ParseCSV<WeaponMeshInfo>("Weapon.csv");
}

void EquipmentManager::AddInitUnitEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (int i = 0; i < mpEquipmentMeshVec.size(); i++)
	{
		if (mpEquipmentMeshVec[i].className == _classType && mpEquipmentMeshVec[i].buffLv == 0)
		{
			if (i % 14 == 8)
			{
				for (const auto& weaponMesh : mpWeaponMeshVec)
				{
					if (weaponMesh.className == _classType)
					{
						// 무기를 끼우자.
						WearWeapon(_entity, weaponMesh, false);
					}
				}
			}
			TestWearEquipment(_entity, mpEquipmentMeshVec[i], mEquipPart[i % 14].second, true); // 없는 공간은 null이라 기재한다.
			if (_entity->GetComponent<EquipmentComponent>().mpEquipments.size() > 13)
			{
				break;
			}
		}
	}

}

void EquipmentManager::AddNPCEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (int i = 0; i < mpEquipmentMeshVec.size(); i++)
	{
		if (mpEquipmentMeshVec[i].className == _classType && mpEquipmentMeshVec[i].buffLv == 0)
		{
			TestWearEquipment(_entity, mpEquipmentMeshVec[i], mEquipPart[i % 14].second, true); // 없는 공간은 null이라 기재한다.
			if (_entity->GetComponent<EquipmentComponent>().mpEquipments.size() > 13)
			{
				break;
			}
		}
	}
}

void EquipmentManager::AddUnitEquip(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv)
{
	for (int i = 0; i < mpEquipmentMeshVec.size(); i++)
	{
		if (mpEquipmentMeshVec[i].className == _classType && mpEquipmentMeshVec[i].buffLv == 0)
		{
			TestWearEquipment(_entity, mpEquipmentMeshVec[i], mEquipPart[i % 14].second, false); // 없는 공간은 null이라 기재한다.
			if (_entity->GetComponent<EquipmentComponent>().mpEquipments.size() > 13)
			{
				break;
			}
		}
	}
}

void EquipmentManager::AddWeapon(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (const auto& weaponMesh : mpWeaponMeshVec)
	{
		if (weaponMesh.className == _classType)
		{
			// 무기를 끼우자. 장비는 끼우지 말자.
			WearWeapon(_entity, weaponMesh, false);
		}
	}
}

void EquipmentManager::UpdateEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv)
{
	//auto& beforeEquipVec = _entity->GetComponent<AnimationComponent>().myEntityVec;
	std::vector<EquipmentMeshInfo> afterEquipInfoVec;
	auto& equipComp = _entity->GetComponent<EquipmentComponent>();
	// 이후 버프에 대한 장비 파츠 정보 저장
	for (const auto& equipmentMesh : mpEquipmentMeshVec)
	{
		if (equipmentMesh.className == _classType && equipmentMesh.buffLv == _buffLv)
		{
			afterEquipInfoVec.push_back(equipmentMesh);
		}
	}

	for (size_t i = 0; i < afterEquipInfoVec.size(); i++)
	{
		// 	std::string a = beforeEquipVec[i]->GetComponent<MeshRenderer>().mFile;
		// 	// [교체] 장비 메쉬가 다른 경우
		// 	auto& beforeEquipFbx = beforeEquipVec[i]->GetComponent<MeshRenderer>().mFile;
		auto& beforeEquipFbx = equipComp.mpEquipments[i]->mModelFileName;
		if (beforeEquipFbx != afterEquipInfoVec[i].equipmentFBXname)
		{
			equipComp.mpEquipments[i]->mModelFileName = afterEquipInfoVec[i].equipmentFBXname;
			equipComp.mpEquipments[i]->mMeshName = afterEquipInfoVec[i].equipmentMeshname;
			equipComp.mpEquipments[i]->mDiffuseTexture = afterEquipInfoVec[i].textureName;
			mpRenderManager->InitailizeEntity(_entity);
		}
		// 		else // [스킵]장비 메쉬가 같은 경우 
		// 		{
		// 			continue;
		// 		}
	}

}

void EquipmentManager::TestWearEquipment(std::shared_ptr<Entity> _entity, const EquipmentMeshInfo& _info, const std::string& _targetBoneName, bool _isInitalize)
{
	// auto equipment = mpEntityManager->CreateEntity("equipment");
	// equipment->AddComponent<Transform>();
	// equipment->GetComponent<Transform>().mpParent = &(_entity->GetComponent<Transform>());
	// equipment->AddComponent<MeshRenderer>(_info.equipmentFBXname, _info.equipmentMeshname, false, false);
	// equipment->AddComponent<Texture3D>(_info.textureName);
	// equipment->AddComponent<TargetBoneComponent>();
	// auto& targetBoneComponent = equipment->GetComponent<TargetBoneComponent>();
	// auto& meshName = _entity->GetComponent<MeshRenderer>().mMeshName;
	// targetBoneComponent.mTargetBoneIndex = mpResourceManager->Get_TargetModelBoneIndex(meshName, _targetBoneName);
	// targetBoneComponent.mTarGetBoneConstantBufferData = &(_entity->GetComponent<BoneGroupComponent>().mBoneData);
	// targetBoneComponent.mTargetBoneData.targrtBoneMatrix = targetBoneComponent.mTarGetBoneConstantBufferData->bone[targetBoneComponent.mTargetBoneIndex];
	// auto& equipVec = _entity->GetComponent<AnimationComponent>().myEntityVec;
	// equipVec.push_back(equipment);
	// 
	// if (_isInitalize == false)
	// {
	// 	// 엔티티 추가 후 초기화를 한 번 해준다.
	// 	mpRenderManager->InitailizeEntity(equipment);
	// }

	auto& equip = _entity->GetComponent<EquipmentComponent>();
	equip.AddEquipment(_info.equipmentFBXname, _info.equipmentMeshname
		, _entity->GetComponent<MeshRenderer>().mMeshName, _targetBoneName
		, _info.textureName);
}

void EquipmentManager::WearWeapon(std::shared_ptr<Entity> _entity, const WeaponMeshInfo& _info, bool _isInitalize)
{
	auto meleeWeapon = mpEntityManager->CreateEntity("meleeWeapon");
	auto& transform = meleeWeapon->AddComponent<Transform>();
	meleeWeapon->GetComponent<Transform>().mpParent = &(_entity->GetComponent<Transform>());
	// 	meleeWeapon->AddComponent<MeshRenderer>(_info.weaponFBXname, _info.weaponMeshname);
	// 	meleeWeapon->GetComponent<MeshRenderer>().mIsWeapon = true;
	meleeWeapon->AddComponent<Texture3D>(_info.textureName);
	auto& meshName = _entity->GetComponent<MeshRenderer>().mMeshName;
	_entity->AddComponent<WeaponComponent>(/*meshName, "RigRPalm", 1.f*/);
	auto& weapon = _entity->GetComponent<WeaponComponent>();
	weapon.mBoneIndex = mpResourceManager->Get_TargetModelBoneIndex(meshName, "RigRPalm");
	weapon.mWeaponAABB = mpResourceManager->Get_AABB(_info.weaponFBXname);
	weapon.mpAttachedEntity = meleeWeapon.get();
	weapon.mFileName = _info.weaponFBXname;
	weapon.mMeshName = _info.weaponMeshname;

	auto& collider = meleeWeapon->AddComponent<BoxCollider>(true);
	collider.mSize = weapon.mWeaponAABB.mMax - weapon.mWeaponAABB.mMin;
	//meleeWeapon->GetComponent<BoxCollider>().mSize *= 0.5f;
	meleeWeapon->AddComponent<Rigidbody>(1.f, 0.f, 0.f, false, true, 0.f, 0.f, 0.f, false);
	// 	meleeWeapon->AddComponent<TargetBoneComponent>();
	//auto& equipVec = _entity->GetComponent<AnimationComponent>().myEntityVec;
	//equipVec.push_back(meleeWeapon);
	if (_entity->HasComponent<PlayerComponent>())
	{
		mpPhysicsManager->AddPhysicsObject(meleeWeapon->GetUID(), TYPE_WEAPON, ATTR_ALLY);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		mpPhysicsManager->AddPhysicsObject(meleeWeapon->GetUID(), TYPE_WEAPON, ATTR_ENEMY);
	}
	else if (_entity->HasComponent<NPCComponent>())
	{
		mpPhysicsManager->AddPhysicsObject(meleeWeapon->GetUID()/*, TYPE_WEAPON, ATTR_ENEMY*/);
	}
	//if (_isInitalize == false)
	{
		// 엔티티 추가 후 초기화를 한 번 해준다.
		mpRenderManager->InitailizeEntity(_entity);
	}
}
