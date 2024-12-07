#pragma once
#include "Component.h"
#include "EasingType.h"
#include "EasingFunc.h"

/// <summary>
/// 트위닝을 위한 컴포넌트
/// </summary>
struct EasingComponent
	: public Component
{
public:
	EasingComponent(std::shared_ptr<Entity> _owner, EasingFunc* _pEasingFunc)
		: Component(_owner), m_pEasingFunc(_pEasingFunc)
	{
	}

	// 트위닝 리스트에 값을 추가하는 함수
	void AddTween(float _start, float _end, float* _value, EasingType _type)
	{
		m_tweeningList.emplace_back(_start, _end, _value, _type);  // 값 자체를 저장
	}

	// 트위닝 리스트 업데이트 함수
	void Update(size_t _index, float _dTime)
	{
		// 인덱스 범위 검사
		if (_index < 0 || _index >= m_tweeningList.size())
		{
			DLOG(LOG_ERROR, "Error: Tween index out of range.");
			return;
		}

		// 포인터 검사
		if (!m_pEasingFunc)
		{
			DLOG(LOG_ERROR, "Error: Easing function pointer is nullptr");
			return;
		}

		m_dTime += _dTime;
		auto& [start, end, value, type] = m_tweeningList[_index];

		// easing type이 유효한지 검사
		if (m_pEasingFunc->EasingFuncMap.find(GetEasingTypeString(type)) == m_pEasingFunc->EasingFuncMap.end())
		{
			DLOG(LOG_ERROR, "Error: Easing function not found for type: " + GetEasingTypeString(type));
			return;
		}

		// 업데이트 콜백 호출
		m_pEasingFunc->EasingFuncMap[GetEasingTypeString(type)](start, end, m_dTime, value);
	}

private:
	// EasingType을 문자열로 변환하는 함수
	std::string GetEasingTypeString(EasingType type) const
	{
		switch (type)
		{
		case EasingType::InSine: return "easeInSine";
		case EasingType::OutSine: return "easeOutSine";
		case EasingType::InOutSine: return "easeInOutSine";
		case EasingType::InCubic: return "easeInCubic";
		case EasingType::OutCubic: return "easeOutCubic";
		case EasingType::InOutCubic: return "easeInOutCubic";
		case EasingType::InQuint: return "easeInQuint";
		case EasingType::OutQuint: return "easeOutQuint";
		case EasingType::InOutQuint: return "easeInOutQuint";
		case EasingType::InCirc: return "easeInCirc";
		case EasingType::OutCirc: return "easeOutCirc";
		case EasingType::InOutCirc: return "easeInOutCirc";
		case EasingType::InElastic: return "easeInElastic";
		case EasingType::OutElastic: return "easeOutElastic";
		case EasingType::InOutElastic: return "easeInOutElastic";
		case EasingType::InBack: return "easeInBack";
		case EasingType::OutBack: return "easeOutBack";
		case EasingType::InOutBack: return "easeInOutBack";
		case EasingType::InBounce: return "easeInBounce";
		case EasingType::OutBounce: return "easeOutBounce";
		case EasingType::InOutBounce: return "easeInOutBounce";
		default: return "unknown";
		}
	}
public:
	EasingFunc* m_pEasingFunc;
	float m_dTime = 0.f;
	std::vector<std::tuple<float, float, float*, EasingType>> m_tweeningList;

};
