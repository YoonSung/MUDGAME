#pragma once

#include "Monster.h"
#include "PlayerCharacter.h"
#define MAX_MONSTER_NUM 10

class CRoom
{
public:
	CRoom(CMonster* monster, CPlayerCharacter* player);
	~CRoom(void);
	void addMonster(CMonster monster);
private:
	CRoom(void);
	CPlayerCharacter* m_Player;
	CMonster* m_Monster [ MAX_MONSTER_NUM ];
	void StartCatchMeIfYouCan();
};

