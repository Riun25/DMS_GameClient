#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

struct ParticleDesc
{
	Vector3 position;
	float playTime;
	Vector3 color;
	float lifeTime;
	Vector3 direction;
	float velocity;
	Vector3 Rotation;
	float opacity;
	Vector3 Scale;
	float deltaOpacity;
	Vector3 deltaRotation;
	float SclRandomFactor;
	Vector3 deltaScale;
	Vector3 PosRandomFactor;
	Vector3 RotRandomFactor;
	Vector3 dirRandomFactor;
	Vector3 deltaColor;
	float gravity;
	Vector3 colorRandomFactor;
};

/// <summary>
/// ParticleComponent(Vec3 위치, float 진행시간, Vec3 색상, float 총 시간, Vec3 방향, float 속도
/// , Vec3 회전값, float 투명도, Vec3 스케일, float 투명도 변화량, Vec3 회전값 변화량, float 스케일 랜덤값
/// , Vec3 스케일 변화량, Vec3 위치 랜덤값, Vec3 회전 랜덤값, Vec3 방향 랜덤값, Vec3 색상 변화량
/// , float 중력계수, Vec3 색상 랜덤값
/// </summary>
struct ParticleComponent
	: public Component
{
public:
	ParticleComponent(std::shared_ptr<Entity> _owner, const Vector3& _position = Vector3(), float _playTime = 0
		, const Vector3& _color = Vector3(1), float _lifeTime = 4, const Vector3& _direction = Vector3(), float _velocity = 0
		, const Vector3& _rotation = Vector3(), float _opacity = 1, const Vector3& _scale = Vector3(1)
		, float _dOpacity = 0, const Vector3& _dRotation = Vector3(), float _scaleRandomFactor = 0
		, const Vector3& _dScale = Vector3(0.5), const Vector3& _posRandomFactor = Vector3(), const Vector3& _rotRandomFactor = Vector3()
		, const Vector3& _dirRandomFactor = Vector3(), const Vector3& _dColor = Vector3(), float _gravity = 0
		, const Vector3& _colorRandomFactor = Vector3())
		: Component(_owner), m_pParticleData(std::make_shared<CSParticleData>())
	{
		m_pParticleData->position = _position;
		m_pParticleData->playTime = _playTime;
		m_pParticleData->color = _color;
		m_pParticleData->lifeTime = _lifeTime;
		m_pParticleData->direction = _direction;
		m_pParticleData->velocity = _velocity;
		m_pParticleData->Rotation = _rotation;
		m_pParticleData->opacity = _opacity;
		m_pParticleData->Scale = _scale;
		m_pParticleData->deltaOpacity = _dOpacity;
		m_pParticleData->deltaRotation = _dRotation;
		m_pParticleData->SclRandomFactor = _scaleRandomFactor;
		m_pParticleData->deltaScale = _dScale;
		m_pParticleData->PosRandomFactor = _posRandomFactor;
		m_pParticleData->RotRandomFactor = _rotRandomFactor;
		m_pParticleData->dirRandomFactor = _dirRandomFactor;
		m_pParticleData->deltaColor = _dColor;
		m_pParticleData->gravity = _gravity;
		m_pParticleData->colorRandomFactor = _colorRandomFactor;
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ParticleComponent>(*this);
	}

public:
	std::shared_ptr<CSParticleData> m_pParticleData;
};