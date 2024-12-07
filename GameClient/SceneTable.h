#pragma once
#include "Logger.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>

struct SceneTable
{
	uint8_t sceneNum;
	std::string sceneName;
	uint32_t sceneMoney;
	uint32_t standardRankA;
	uint32_t standardRankB;
	float time;

	SceneTable(const std::vector<std::string>& row)
	{
		try
		{
			sceneNum = static_cast<uint8_t>(std::stoi(row[0]));
			sceneMoney = static_cast<uint32_t>(std::stoi(row[1]));
			standardRankA = static_cast<uint32_t>(std::stoi(row[2]));
			standardRankB = static_cast<uint32_t>(std::stoi(row[3]));
			time = std::stof(row[4]);
			sceneName = row[5];
		}
		catch (const std::exception& e)
		{
			DLOG(LOG_INFO, "Exception parsing data for scene " + std::to_string(sceneNum) + std::string(": ") + std::string(e.what()));
		}
	}
};