#pragma once
#include <string>
#include <vector>
#include <iostream>

struct CharacterMeshInfo
{
	std::string className;
	int buffLv;
	std::string fbxName;
	std::string meshName;
	std::string textureName;

	CharacterMeshInfo(const std::vector<std::string>& _row)
	{
		try
		{
			className = _row[0];
			fbxName = _row[1];
			meshName = _row[2];
			textureName = _row[3];
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << className << ": " << e.what() << std::endl;
		}
	}
};