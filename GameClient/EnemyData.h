#pragma once
#include <string>
#include <vector>
#include <iostream>

struct EnemyData
{
	int sceneNum;
	std::string className;
	float posX;
	float posZ;

	// 기본 생성자
	EnemyData()
		:sceneNum(-1), className(""), posX(0.f), posZ(0.f)
	{}

	EnemyData(const std::vector<std::string>& _row)
	{
		try
		{
			sceneNum = std::stof(_row[0]);
			className = _row[1];
			posX = std::stof(_row[2]);
			posZ = std::stof(_row[3]);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << className << ": " << e.what() << std::endl;
		}
	}
};