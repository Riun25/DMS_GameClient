#pragma once
#include "Component.h"

/// <summary>
/// 새로운 컴포넌트들 정의하려면 Component 클래스를 상속
/// </summary>
struct SampleComponent :
    public Component
{
public:
    int m_value;
};

