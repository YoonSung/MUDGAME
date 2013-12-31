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
	void CheckZombieCapture();
	int getMonsterNumberInRoom();
	void CRoom::AttackPlayer(int damage);
	bool IsPlayerDead();
	void SynchronizePlayer(int level, int energy, int experience, int killingNum);
private:
	static CRoom* instance;
	CPlayerCharacter* m_MockPlayer;
	CMonster* m_Monster [ MAX_MONSTER_NUM ];
private:
	CRoom(void);
	void destroyMonster();
	int m_CurrentMonsterNumber;
};

