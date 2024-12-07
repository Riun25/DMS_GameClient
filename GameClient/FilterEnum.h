#pragma once

enum CollisionType	// word0
{
	TYPE_GROUND = 1 << 0, // 땅
	TYPE_UNIT = 1 << 1, // 유닛
	TYPE_PROJECTILE = 1 << 2, // 투사체
	TYPE_WEAPON = 1 << 3, // 무기
	TYPE_GOLD = 1 << 4, // 금화
	TYPE_OBSTACLE = 1 << 5  // 장애물
};

enum CollisionAttribute	// word1
{
	ATTR_ALLY = 1 << 0, // 아군
	ATTR_ENEMY = 1 << 1, // 적군
	ATTR_ON_GROUND = 1 << 2, // 지면에 붙어 있는지 여부
	ATTR_IS_DEAD = 1 << 3
};