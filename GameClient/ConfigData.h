#pragma once
#include "IData.h"
class ConfigData :
	public IData
{
public:
	float GetVolumeMaster() const { return m_volumeMaster; }
	void SetVolumeMaster(float val) { m_volumeMaster = val; }
	float GetVolumeBGM() const { return m_volumeBGM; }
	void SetVolumeBGM(float val) { m_volumeBGM = val; }
	float GetVolumeSFX() const { return m_volumeSFX; }
	void SetVolumeSFX(float val) { m_volumeSFX = val; }
	float GetFPS() const { return m_FPS; }
	void SetFPS(float val) { m_FPS = val; }

private:
	// 오디오 설정
	float m_volumeMaster;
	float m_volumeBGM;
	float m_volumeSFX;

	// 프레임 설정
	float m_FPS;
};

