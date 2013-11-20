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
	CGameMap(void);
	~CGameMap(void);

	MapInfo* GetMapInfo ( int x, int y );

protected:
	MapInfo m_MapInfo[MAP_SIZE][MAP_SIZE];
};

