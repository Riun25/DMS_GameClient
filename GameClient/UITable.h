#pragma once
#include <string>
#include <vector>
#include <iostream>

struct UITable
{
	std::string entityID;
	std::string UItype;
	bool isCombine;
	std::string text;
	std::string fontName;
	std::string textureName;
	std::string changeTexture;
	float posX; 
	float posY; 
	float width; 
	float height; 
	int layer; 
	bool isVisible; 
	float r; 
	float g;
	float b; 
	float a;
	float fontScale;
	int textNum1;
	int textNum2;
	float H_r;
	float H_g;
	float H_b;
	float H_a;
	float P_r;
	float P_g;
	float P_b;
	float P_a;

	UITable(const std::vector<std::string>& _row)
	{
		try
		{
			entityID = _row[0];
			UItype = _row[1];
			isCombine = (_row[2] == "1");
			text = _row[3];
			fontName = _row[4];
			textureName = _row[5];
			changeTexture = _row[6];
			fontName = _row[7];
			posX = std::stof(_row[8]);
			posY = std::stof(_row[9]);
			width = std::stof(_row[10]);
			height = std::stof(_row[11]);
			layer = std::stof(_row[12]);
			isVisible = std::stof(_row[13]);
			r = std::stof(_row[14]);
			g = std::stof(_row[15]);
			b = std::stof(_row[16]);
			a = std::stof(_row[17]);
			fontScale = std::stof(_row[18]);
			textNum1 = std::stof(_row[19]);
			textNum2 = std::stof(_row[20]);
			H_r = std::stof(_row[21]);
			H_g = std::stof(_row[22]);
			H_b = std::stof(_row[23]);
			H_a = std::stof(_row[24]);
			P_r = std::stof(_row[25]);
			P_g = std::stof(_row[26]);
			P_b = std::stof(_row[27]);
			P_a = std::stof(_row[28]);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << entityID << ": " << e.what() << std::endl;
		}
	}
};