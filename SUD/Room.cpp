#include "stdafx.h"
#include "Room.h"
#include "Monster.h"

CRoom* CRoom::instance;
//void destroyMonster()

CRoom* CRoom::getInstancePtr()
{
	if ( instance == nullptr )
	{
		instance = new CRoom();
	}
	return instance;
}

void CRoom::releaseInstance()
{
	delete instance;
}

CPlayerCharacter* CRoom::getPlayer()
{
	return m_Player;
}

bool CRoom::addMonster(CMonster& monster)
{
	bool isAdded = false;
	
	Position monster_pos = monster.GetPosition();
	MapInfo* tempMapInfo = GetMapInfo( monster_pos.x, monster_pos.y);

	for ( int index = 0 ; index < MAX_MONSTER_NUM ; ++index )
	{
		if ( tempMapInfo->pMob == nullptr )
		{
			tempMapInfo->pMob = &monster;
			isAdded = true;
			break;
		}
	}

	return isAdded;
}


CRoom::CRoom(void)
{
	m_Player = new CPlayerCharacter();
	m_Player->SetPosition(4,5);
}

void CRoom::StartCatchMeIfYouCan()
{
}

CRoom::~CRoom(void)
{
	delete m_Player;
}
