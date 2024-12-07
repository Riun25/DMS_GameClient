#include "UserData.h"
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void UserData::Load() 
{
	if (!std::filesystem::exists(m_filePath)) 
	{
		Save();  // 파일이 없을 경우 기본 설정으로 파일 생성
		return;
	}

	std::ifstream inputFile(m_filePath);
	if (!inputFile.is_open()) return;

	json j;
	inputFile >> j;

	// Graphics Section
	if (j.contains("graphics"))
	{
		auto& g = j["graphics"];
		graphics.screenResolution = g.value("screenResolution", graphics.screenResolution);
		graphics.fullscreen = g.value("fullscreen", graphics.fullscreen);
		graphics.vsync = g.value("vsync", graphics.vsync);
		graphics.drawShadow = g.value("drawShadow", graphics.drawShadow);
	}

	// Audio Section
	if (j.contains("audio")) 
	{
		auto& a = j["audio"];
		audio.masterVolume = a.value("masterVolume", audio.masterVolume);
		audio.bgmVolume = a.value("bgmVolume", audio.bgmVolume);
		audio.sfxVolume = a.value("sfxVolume", audio.sfxVolume);
	}

	// Config Section
	if (j.contains("config")) 
	{
		config.fps = j["config"].value("fps", config.fps);
	}

	// Gameplay Section
	if (j.contains("gameplay"))
	{
		auto& g = j["gameplay"];
		gameplay.moneyAmount = g.value("moneyAmount", gameplay.moneyAmount);
		gameplay.moneySpeed = g.value("moneySpeed", gameplay.moneySpeed);
		gameplay.projectileSpeed = g.value("projectileSpeed", gameplay.projectileSpeed);
	}

	// Debug Section
	if (j.contains("debug")) 
	{
		auto& d = j["debug"];
		debug.drawCollider = d.value("drawCollider", debug.drawCollider);
		debug.drawFps = d.value("drawFps", debug.drawFps);
		debug.drawCameraInfo = d.value("drawCameraInfo", debug.drawCameraInfo);
		debug.flyingCamera = d.value("flyingCamera", debug.flyingCamera);
		debug.inputSceneChange = d.value("inputSceneChange", debug.inputSceneChange);
		debug.logging = d.value("logging", debug.logging);
	}
}

void UserData::Save() const
{
	json j;

	// Graphics Section
	j["graphics"]["screenResolution"] = graphics.screenResolution;
	j["graphics"]["fullscreen"] = graphics.fullscreen;
	j["graphics"]["vsync"] = graphics.vsync;
	j["graphics"]["drawShadow"] = graphics.drawShadow;

	// Audio Section
	j["audio"]["masterVolume"] = audio.masterVolume;
	j["audio"]["bgmVolume"] = audio.bgmVolume;
	j["audio"]["sfxVolume"] = audio.sfxVolume;

	// Config Section
	j["config"]["fps"] = config.fps;

	// Gameplay Section
	j["gameplay"]["moneyAmount"] = gameplay.moneyAmount;
	j["gameplay"]["moneySpeed"] = gameplay.moneySpeed;
	j["gameplay"]["projectileSpeed"] = gameplay.projectileSpeed;

	// Debug Section
	j["debug"]["drawCollider"] = debug.drawCollider;
	j["debug"]["drawFps"] = debug.drawFps;
	j["debug"]["drawCameraInfo"] = debug.drawCameraInfo;
	j["debug"]["flyingCamera"] = debug.flyingCamera;
	j["debug"]["inputSceneChange"] = debug.inputSceneChange;
	j["debug"]["logging"] = debug.logging;

	std::ofstream outputFile(m_filePath);
	outputFile << j.dump(4);  // JSON 파일을 예쁘게 출력 (인덴트 4)
}
