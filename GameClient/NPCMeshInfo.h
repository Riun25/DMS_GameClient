#pragma once
#include <string>
#include <vector>
#include <iostream>

struct NPCMeshInfo
{
	std::string npcName;
	float posX;
	float posY;
	float posZ;
	float rotY;
	float scale;
	std::string fbxName;
	std::string meshName;
	std::string textureName;
	bool hasWeapon;

	NPCMeshInfo(const std::vector<std::string>& _row)
	{
		try
		{
			npcName = _row[0];
			posX = std::stof(_row[1]);
			posY = std::stof(_row[2]);
			posZ = std::stof(_row[3]);
			rotY = std::stof(_row[4]);
			scale = std::stof(_row[5]);
			fbxName = _row[6];
			meshName = _row[7];
			textureName = _row[8];
			hasWeapon = (_row[9] == "1");
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << npcName << ": " << e.what() << std::endl;
		}
	}
};