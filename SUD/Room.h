#pragma once

#include "Monster.h"
#include "GameMap.h"
#include "PlayerCharacter.h"
#define MAX_MONSTER_NUM 15

class CRoom
: public CGameMap
{
public:
	static CRoom* getInstancePtr();
	static void releaseInstance();
	bool addMonster(CMonster& monster);
	~CRoom(void);
	CPlayerCharacter* CRoom::getPlayer();
private:
	static CRoom* instance;
	CPlayerCharacter* m_Player;
	CMonster* m_Monster [ MAX_MONSTER_NUM ];
private:
	CRoom(void);
	void StartCatchMeIfYouCan();
	void destroyMonster();
};

