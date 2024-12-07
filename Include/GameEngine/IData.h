#pragma once
#include "nlohmann/json.hpp"
using Json = nlohmann::json;

/// <summary>
/// 시리얼라이즈 할 데이터들의 인터페이스 클래스입니다.
/// </summary>
class IData abstract
{
public:
	virtual ~IData() = default;

// 	virtual void Serialize() abstract;
// 	virtual void Deserialize() abstract;

};

