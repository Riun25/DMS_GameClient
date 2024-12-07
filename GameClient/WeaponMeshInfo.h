#pragma once
#include <string>
#include <vector>
#include <iostream>

struct WeaponMeshInfo
{
	std::string className;
	std::string weaponFBXname;
	std::string weaponMeshname;
	std::string textureName;

	// 기본 생성자 -> 장비 
	WeaponMeshInfo()
		:className(""), weaponFBXname(""), weaponMeshname(""), textureName("") {}

	WeaponMeshInfo(const std::vector<std::string>& _row)
	{
		try
		{
			className = _row[0];
			weaponFBXname = _row[1];
			weaponMeshname = _row[2];
			textureName = _row[3];
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << className << ": " << e.what() << std::endl;
		}
	}
};