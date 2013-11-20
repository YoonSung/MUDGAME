#include "stdafx.h"
#include "GameMap.h"


//#define ZERO_MEMORY(mem, size) memset(mem, 0, size)

MapInfo mapInfo;

CGameMap::CGameMap(void)
{
	memset(m_MapInfo, 0, sizeof(m_MapInfo));
	//ZERO_MEMORY(m_MapInfo, sizeof(m_MapInfo);
}


CGameMap::~CGameMap(void)
{
	for ( int i = 0; i < MAP_SIZE; ++i )
	{
		for ( int j = 0; j < MAP_SIZE; ++j )
		{
			delete m_MapInfo[i][j].pMob;
			m_MapInfo[i][j].pMob = nullptr;
		}
	}

}

MapInfo* CGameMap::GetMapInfo( int x, int y )
{
	if ( x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE )
		return nullptr;

	return &m_MapInfo[x][y];
}
