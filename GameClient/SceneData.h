#pragma once
#include "IData.h"

class SceneData
	: public IData
{
public:

public:
	float m_totalTime = 90.f;
	float m_time = 90.f;			// 남은 전투 시간
	uint8_t m_aliveAlly = 0;	// 생존한 아군 용병 수
	uint8_t m_totalAlly = 0;	// 생존한 아군 용병 수
	uint32_t m_heldMoney = 3000;	// 보유 금액
	uint32_t m_usedAmount = 0;	// 사용된 금액
	uint32_t m_remainAmount = 0;// 보상 금액
	uint32_t m_totalAmount = 3000;

	uint16_t m_rankA = 1000;
	uint16_t m_rankB = 500;

};

