#pragma once
#include "pch.h"
#include <random>
inline constexpr int MAX_FILE_LENGTH = 256;

using UID = uint32_t;

/// <summary>
/// 문자열, 파일 시스템등의 유틸리티 함수들을 모아둔 클래스입니다.
/// </summary>
class Utility
{
public:
	// std::wstring을 std::string으로 변환합니다. 
	static std::string ToString(const std::wstring_view _value);

	static std::string ToString(const TCHAR* _value);

	static std::wstring ToWString(const std::string_view _value);

	/// <summary>
	/// 파일 다이얼로그를 열고 선택된 파일 경로를 반환합니다.
	/// </summary>
	/// <returns>선택된 파일 경로</returns>
	static std::wstring OpenFileDialog();
};

class UIDGenerator
{
public:
	template <typename T>
	static UID GenerateUID()
	{
		static std::atomic<UID> counter(0);
		return counter++;
	}

};

class RandomUtil
{
public:
	static int RandomInt(int _min, int _max);
	static float RandomFloat(float _min, float _max);
};