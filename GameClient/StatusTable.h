#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct StatusTable
{
	std::string className;
	int buffLv;
	int placeMoney;
	float unitSize;
	float maxHP;
	float attackPower;
	float attackSpeed;
	float moveSpeed;
	float attackRange;
	float recognitionRange;
	float buffSpeed;
	float buffMoney1;
	float buffMoney2;

	StatusTable(const std::vector<std::string>& row)
	{
		try
		{
			className = row[0];  // ¹®ÀÚ¿­ Ä®·³
			buffLv = std::stoi(row[1]);
			placeMoney = std::stoi(row[2]);
			unitSize = std::stof(row[3]);
			maxHP = std::stof(row[4]);
			attackPower = std::stof(row[5]);
			attackSpeed = std::stof(row[6]);
			moveSpeed = std::stof(row[7]);
			attackRange = std::stof(row[8]);
			recognitionRange = std::stof(row[9]);
			buffSpeed = std::stof(row[10]);
			buffMoney1 = std::stof(row[11]);
			buffMoney2 = std::stof(row[12]);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << className << ": " << e.what() << std::endl;
		}
	}
};