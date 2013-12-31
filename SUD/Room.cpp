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

CRoom::~CRoom(void)
{
	delete m_Player;
}

bool CRoom::isCatchZombieSuccess()
{
	int x = m_Player->GetPosition().x;
	int y = m_Player->GetPosition().y;

	MapInfo* mapInfo = GetMapInfo(x, y);
	if (  mapInfo->pMob != nullptr )
	{
		CMonster* test = mapInfo->pMob;
		//delete test; 메모리누수. 왜 에러가 나는지 모르겠습니다.. block_type_is_invalid type ...
		mapInfo->pMob = nullptr;
		return true;
	}

	return false;
}