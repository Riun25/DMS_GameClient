#pragma once
#include "pch.h"
using Json = nlohmann::json;

class JsonUtil
{
public:
	void Serialize(const std::wstring& _file, const Json& _value)
	{
		std::ofstream output(_file);

		if (!output.is_open())
		{
			assert(false);
		}
		else
		{
			output << _value.dump();
			output.close();
		}
	}
	
	void Deserialize(const std::wstring& _file, Json& _value) 
	{
		std::ifstream input(_file);

		if (!input.is_open())
		{
			assert(false);
		}
		else
		{
			input >> _value;
			input.close();
		}
	}

};