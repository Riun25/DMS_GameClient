#pragma once
#include "Component.h"
#include "EasingType.h"
#include "EasingFunc.h"

/// <summary>
/// Ʈ������ ���� ������Ʈ
/// </summary>
struct EasingComponent
	: public Component
{
public:
	EasingComponent(std::shared_ptr<Entity> _owner, EasingFunc* _pEasingFunc)
		: Component(_owner), mpEasingFunc(_pEasingFunc)
	{
	}

	// Ʈ���� ����Ʈ�� ���� �߰��ϴ� �Լ�
	void AddTween(float _start, float _end, float* _value, EasingType _type)
	{
		mtweeningList.emplace_back(_start, _end, _value, _type);  // �� ��ü�� ����
	}

	// Ʈ���� ����Ʈ ������Ʈ �Լ�
	void Update(size_t _index, float _dTime)
	{
		// �ε��� ���� �˻�
		if (_index < 0 || _index >= mtweeningList.size())
		{
			DLOG(LOG_ERROR, "Error: Tween index out of range.");
			return;
		}

		// ������ �˻�
		if (!mpEasingFunc)
		{
			DLOG(LOG_ERROR, "Error: Easing function pointer is nullptr");
			return;
		}

		mdTime += _dTime;
		auto& [start, end, value, type] = mtweeningList[_index];

		// easing type�� ��ȿ���� �˻�
		if (mpEasingFunc->EasingFuncMap.find(GetEasingTypeString(type)) == mpEasingFunc->EasingFuncMap.end())
		{
			DLOG(LOG_ERROR, "Error: Easing function not found for type: " + GetEasingTypeString(type));
			return;
		}

		// ������Ʈ �ݹ� ȣ��
		mpEasingFunc->EasingFuncMap[GetEasingTypeString(type)](start, end, mdTime, value);
	}

private:
	// EasingType�� ���ڿ��� ��ȯ�ϴ� �Լ�
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
	EasingFunc* mpEasingFunc;
	float mdTime = 0.f;
	std::vector<std::tuple<float, float, float*, EasingType>> mtweeningList;

};
