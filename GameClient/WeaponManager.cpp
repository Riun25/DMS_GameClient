#include "WeaponManager.h"
#include "TargetBoneComponent.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "RenderManager.h"
#include "WeaponCollider.h"
#include "directxtk/GeometricPrimitive.h"

WeaponManager::WeaponManager(entt::registry& _registry, RenderManager* _renderManager)
	: mRegistry(_registry), mpRenderManager(_renderManager)
{

}

bool WeaponManager::Initialize()
{
	return true;
}

void WeaponManager::FixedUpdate(float _fixedDTime)
{
	// 	auto view = mRegistry.view<Rigidbody, WeaponComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto rigidbody = mRegistry.try_get<Rigidbody>(entity);
	// 		auto weapon = mRegistry.try_get<WeaponComponent>(entity);
	// 
	// 		Vector3 pos = Vector3(weapon->mPosition.x, weapon->mPosition.y, weapon->mPosition.z);
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
// 	auto view = mRegistry.view<BoxCollider, TargetBoneComponent>();
// 	for (auto entity : view)
// 	{
// 		auto collider = mRegistry.try_get<BoxCollider>(entity);
// 		auto target = mRegistry.try_get<TargetBoneComponent>(entity);
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
	// 	auto view = mRegistry.view<Transform, WeaponComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto transform = mRegistry.try_get<Transform>(entity);	// mLocalPosition, mLocalRotation, mLocalScale;
	// 		auto weapon = mRegistry.try_get<WeaponComponent>(entity);
	// 
	// 		auto& child = weapon->mpAttachedEntity->GetComponent<TargetBoneComponent>();
	// 		
	// 		child.mTargetBoneAboveMatrix = mpRenderManager->m_pGraphicsEngine->
	// 				GetTargetBoneAboveMatrix(weapon->m_targetModel, weapon->m_targetBone, weapon->mScale);	// Matrix
			//child.mpOwner->GetComponent<Transform>().m_localMatrix = child.mTargetBoneAboveMatrix;
	// 	}
}

void WeaponManager::LateUpdate(float _dTime)
{
	// 	auto view = mRegistry.view<Transform, BoxCollider, WeaponComponent, TargetBoneComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		auto transform = mRegistry.try_get<Transform>(entity);	// mLocalPosition, mLocalRotation, mLocalScale;
	// 		auto targetBone = mRegistry.try_get<TargetBoneComponent>(entity);
	// 		auto weapon = mRegistry.try_get<WeaponComponent>(entity);
	// 		auto collider = mRegistry.try_get<BoxCollider>(entity);
	// 		Quaternion qut = Quaternion::CreateFromYawPitchRoll(weapon->m_rotation);
	// 		Matrix matrix = transform->mpParent->GetTransformMatrix();
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
	// 		mpRenderManager->m_pGraphicsEngine->Rend_DebugBox(scale, rotation, position);
	// 	}
}

void WeaponManager::Finalize()
{

}

void WeaponManager::Render()
{
	auto view = mRegistry.view<WeaponCollider>();
	for (auto entity : view)
	{
// 		auto transform = mRegistry.try_get<Transform>(entity);
// 		auto collider = mRegistry.try_get<WeaponCollider>(entity);
// 		// collider->mWeaponAABB.mMax *= 1;
// 		// collider->mWeaponAABB.mMin *= 1;
// 		//auto matrix = mRegistry.try_get<TargetBoneComponent>(entity)->mTargetBoneData.targrtBoneMatrix	* transform->GetTransformMatrix();
// 		auto matrix = transform->GetTransformMatrix();
// 		mpRenderManager->m_pGraphicsEngine->Rend_DebugBox(
// 			Matrix()
// 			, Matrix()
// 			, matrix
// 		);

	}
}
