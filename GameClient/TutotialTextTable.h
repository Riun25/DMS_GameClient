#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct TutotialTextTable
{
	std::u8string text;
	int idx;
	/*float textPosX;
	float textPosY;
	float imgPosX;
	float imgPosY;
	float imgSizeX;
	float imgSizeY;*/

	std::u8string ConvertEnterToString(const std::u8string& _str)
	{
		std::u8string result = _str;
		std::u8string::size_type pos = 0;
		while ((pos = result.find(u8"\\n", pos)) != std::string::npos)
		{
			result.replace(pos, 2, u8"\n");
			pos += 1;
		}
		return result;
	}

	TutotialTextTable(const std::vector<std::string>& row)
	{
		try
		{
			text = ConvertEnterToString(std::u8string(row[0].begin(), row[0].end()));
			idx = std::stoi(row[1]);
			/*textPosX = std::stoi(row[1]);
			textPosY = std::stof(row[2]);
			imgPosX = std::stof(row[3]);
			imgPosY = std::stof(row[4]);
			imgSizeX = std::stof(row[5]);
			imgSizeY = std::stof(row[6]);*/
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << row[0] << ": " << e.what() << std::endl;
		}
	}
};