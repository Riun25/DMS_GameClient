#include "WeaponManager.h"
#include "TargetBoneComponent.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "RenderManager.h"
#include "WeaponCollider.h"
#include "directxtk/GeometricPrimitive.h"

WeaponManager::WeaponManager(entt::registry& _registry, RenderManager* _renderManager)
	: m_registry(_registry), m_pRenderManager(_renderManager)
{

}

bool WeaponManager::Initialize()
{
	return true;
}

void WeaponManager::FixedUpdate(float _fixedDTime)
{
	// 	auto view = m_registry.view<Rigidbody, WeaponComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto rigidbody = m_registry.try_get<Rigidbody>(entity);
	// 		auto weapon = m_registry.try_get<WeaponComponent>(entity);
	// 
	// 		Vector3 pos = Vector3(weapon->m_position.x, weapon->m_position.y, weapon->m_position.z);
	// 		Quaternion qut = Quaternion::CreateFromYawPitchRoll(weapon->m_rotation);
	// 
	// 		if (rigidbody->m_pRigidActor)
	// 		{
	// 			physx::PxTransform psTransform(physx::PxVec3(pos.x, pos.y, pos.z)
	// 				, physx::PxQuat(qut.x, qut.y, qut.z, qut.w)
	// 			); 
	// 			rigidbody->m_pRigidActor->setGlobalPose(psTransform);
	// 		}
	// 	}
// 
// 	auto view = m_registry.view<BoxCollider, TargetBoneComponent>();
// 	for (auto entity : view)
// 	{
// 		auto collider = m_registry.try_get<BoxCollider>(entity);
// 		auto target = m_registry.try_get<TargetBoneComponent>(entity);
// 
// 		collider->m_center = Vector3(
// 			target->mTargetBoneAboveMatrix._41
// 			, target->mTargetBoneAboveMatrix._42
// 			, target->mTargetBoneAboveMatrix._43
// 		);
// 
// 	}

}

void WeaponManager::Update(float _dTime)
{
	// 	auto view = m_registry.view<Transform, WeaponComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto transform = m_registry.try_get<Transform>(entity);	// m_localPosition, m_localRotation, m_localScale;
	// 		auto weapon = m_registry.try_get<WeaponComponent>(entity);
	// 
	// 		auto& child = weapon->m_pAttachedEntity->GetComponent<TargetBoneComponent>();
	// 		
	// 		child.mTargetBoneAboveMatrix = m_pRenderManager->m_pGraphicsEngine->
	// 				GetTargetBoneAboveMatrix(weapon->m_targetModel, weapon->m_targetBone, weapon->m_scale);	// Matrix
			//child.m_pOwner->GetComponent<Transform>().m_localMatrix = child.mTargetBoneAboveMatrix;
	// 	}
}

void WeaponManager::LateUpdate(float _dTime)
{
	// 	auto view = m_registry.view<Transform, BoxCollider, WeaponComponent, TargetBoneComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto transform = m_registry.try_get<Transform>(entity);	// m_localPosition, m_localRotation, m_localScale;
	// 		auto targetBone = m_registry.try_get<TargetBoneComponent>(entity);
	// 		auto weapon = m_registry.try_get<WeaponComponent>(entity);
	// 		auto collider = m_registry.try_get<BoxCollider>(entity);
	// 		Quaternion qut = Quaternion::CreateFromYawPitchRoll(weapon->m_rotation);
	// 		Matrix matrix = transform->m_pParent->GetTransformMatrix();
	// 
	// 		matrix = matrix * targetBone->mTargetBoneAboveMatrix;
	// 
	// 		Vector3 position = Vector3(matrix._41, matrix._42, matrix._43);
	// 		Vector3 scale = Vector3(
	// 			Vector3(matrix._11, matrix._12, matrix._13).Length(),
	// 			Vector3(matrix._21, matrix._22, matrix._23).Length(),
	// 			Vector3(matrix._31, matrix._32, matrix._33).Length()
	// 		);
	// 
	// 		Matrix rotationMatrix = matrix;
	// 
	// 		rotationMatrix._11 /= scale.x;
	// 		rotationMatrix._12 /= scale.x;
	// 		rotationMatrix._13 /= scale.x;
	// 
	// 		rotationMatrix._21 /= scale.y;
	// 		rotationMatrix._22 /= scale.y;
	// 		rotationMatrix._23 /= scale.y;
	// 
	// 		rotationMatrix._31 /= scale.z;
	// 		rotationMatrix._32 /= scale.z;
	// 		rotationMatrix._33 /= scale.z;
	// 
	// 		float pitch = asinf(-rotationMatrix._32);
	// 		float yaw = atan2f(rotationMatrix._31, rotationMatrix._33);
	// 		float roll = atan2f(rotationMatrix._12, rotationMatrix._22);
	// 
	// 		Vector3 rotation = Vector3(pitch, yaw, roll);
	// 
	// 		m_pRenderManager->m_pGraphicsEngine->Rend_DebugBox(scale, rotation, position);
	// 	}
}

void WeaponManager::Finalize()
{

}

void WeaponManager::Render()
{
	auto view = m_registry.view<WeaponCollider>();
	for (auto entity : view)
	{
// 		auto transform = m_registry.try_get<Transform>(entity);
// 		auto collider = m_registry.try_get<WeaponCollider>(entity);
// 		// collider->m_weaponAABB.mMax *= 1;
// 		// collider->m_weaponAABB.mMin *= 1;
// 		//auto matrix = m_registry.try_get<TargetBoneComponent>(entity)->mTargetBoneData.targrtBoneMatrix	* transform->GetTransformMatrix();
// 		auto matrix = transform->GetTransformMatrix();
// 		m_pRenderManager->m_pGraphicsEngine->Rend_DebugBox(
// 			Matrix()
// 			, Matrix()
// 			, matrix
// 		);

	}
}
