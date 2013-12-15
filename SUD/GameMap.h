#pragma once

#include "Monster.h"
#define MAP_SIZE 10

struct MapInfo
{
	CMonster* pMob;
};
class CGameMap
{
public:


	MapInfo* GetMapInfo ( int x, int y );
	static CGameMap* getInstancePtr();
	static void releaseInstance();
protected:
	MapInfo m_MapInfo[MAP_SIZE][MAP_SIZE];
private:
	CGameMap(void);
	~CGameMap(void);
	static CGameMap* instance;
};

