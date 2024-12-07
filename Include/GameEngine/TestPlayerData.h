#pragma once
#include "IData.h"

/// <summary>
/// 유저가 게임 중 저장하는 데이터의 예시 클래스입니다.
/// 유저의 임의 변경을 막기 위해 데이터를 암호화하여 저장합니다.
/// </summary>
class TestPlayerData : public IData
{
public:
    TestPlayerData() {}
    ~TestPlayerData() {}

    void Serialize();
    void Deserialize();

private:
    int m_gold;
    int m_wood;
    int m_iron;
    float m_level;

};

