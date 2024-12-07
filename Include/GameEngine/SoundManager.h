#pragma once
#include "IManager.h"
#include "EventManager.h"
#include "EventListener.h"

class SoundSystem;
class GameEngine;
class EventManager;

/// <summary>
/// FMOD를 이용해서 만든 사운드 매니저 클래스입니다.
/// 추후에 외부 모듈로 만들 예정입니다.
/// 2024.05.20 _Doyo;
/// </summary>
class SoundManager
	: public IManager/*, public EventListener*/
{
public:
	SoundManager() = default;
	//SoundManager(EventManager* _pEventManager);

	~SoundManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override {}
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override {}
	virtual void Finalize() override;

	bool LoadBGM(const std::string& _filename, const std::string& _soundName);
	bool LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop = false);
	bool Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop = false);

public:
	void PlayBGM(const std::string& _soundName);
	void PauseBGM();
	void StopBGM();
	void FadeInBGM(const std::string& _soundName, float _fadeTime);
	void FadeOutBGM(float _fadeTime);

	void PlaySFX(const std::string& _soundName, float _probability = 1.f);
	void StopSFX(const std::string& _soundName);
	void Play3DSound(const std::string& _soundName, float _x, float _y, float _z);

	void SetListenerPosition(float _x, float _y, float _z);
	void SetBGMVolume(float _volume);
	void SetSFXVolume(float _volume);
	void SetMasterVolume(float _volume);

	bool IsPlaying(const std::string& _soundName);

private:
	// 	void PlayBGM(const Event& _event);
	// 	void PauseBGM(const Event& _event);
	// 	void StopBGM(const Event& _event);
	// 	void FadeInBGM(const Event& _event);
	// 	void FadeOutBGM(const Event& _event);
	// 	void PlaySFX(const Event& _event);
	// 	void Play3DSound(const Event& _event);
	// 	void SetListenerPosition(const Event& _event);
	// 	void SetBGMVolume(const Event& _event);
	// 	void SetSFXVolume(const Event& _event);
	// 	void SetMasterVolume(const Event& _event);

private:
	SoundSystem* m_pSoundSystem;
};

