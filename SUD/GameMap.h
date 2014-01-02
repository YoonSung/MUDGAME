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
	void ClearMap();
protected:
	MapInfo m_MapInfo[MAP_SIZE][MAP_SIZE];
	CGameMap(void);
	virtual ~CGameMap(void);
private:
	static CGameMap* instance;
};

