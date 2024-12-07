#pragma once
#include <string>
#include <vector>
#include <iostream>

struct MapData
{
	int sceneNum;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	//float rotZ;
	float scaleX;
	float scaleY;
	float scaleZ;
	std::string FBXname;
	std::string MeshName;
	std::string textureName;
	std::string sceneName;
	bool hasCollider;
	bool alphaBlend;
	bool textureFlow;

	// 기본 생성자
	MapData()
		:sceneNum(-1), posX(0.f), posY(0.f), posZ(0.f), rotX(0.f), rotY(0.f), /*rotZ(0.f)
		,*/ scaleX(0.f), scaleY(0.f), scaleZ(0.f), FBXname(""), MeshName(""), textureName("")
		, hasCollider(false), alphaBlend(false), textureFlow(false)
	{}

	MapData(const std::vector<std::string>& _row)
	{
		try
		{
			sceneNum = std::stof(_row[0]);
			posX = std::stof(_row[1]);
			posY = std::stof(_row[2]);
			posZ = std::stof(_row[3]);
			rotX = std::stof(_row[4]);
			rotY = std::stof(_row[5]);
			//rotZ = std::stof(_row[6]);
			scaleX = std::stof(_row[6]);
			scaleY = std::stof(_row[7]);
			scaleZ = std::stof(_row[8]);
			FBXname = _row[9];
			MeshName = _row[10];
			textureName = _row[11];
			hasCollider = std::stoi(_row[12]);
			alphaBlend = std::stoi(_row[13]);
			textureFlow = std::stoi(_row[14]);
			sceneName = _row[15];
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for scene " << sceneNum << ": " << e.what() << std::endl;
		}
	}
};