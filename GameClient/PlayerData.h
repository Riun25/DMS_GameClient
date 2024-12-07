#pragma once
#include "IData.h"

class PlayerData :
	public IData
{
public:
	uint32_t GetPlayerMoney() const { return m_playerMoney; }
	void SetPlayerMoney(const uint32_t& val) { m_playerMoney = val; }
	uint8_t GetMoneyGunLevel() const { return m_moneyGunLevel; }
	void SetMoneyGunLevel(const uint8_t& val) { m_moneyGunLevel = val; }
	std::unordered_set<uint8_t> GetClearedStatges() const { return m_clearedStatges; }

private:
	uint32_t m_playerMoney;     // 보유중인 돈
	uint8_t m_moneyGunLevel;    // 보유중인 머니건의 레벨;
	std::unordered_set<uint8_t> m_clearedStatges;   // 클리어한 스테이지 해시집합

};

 