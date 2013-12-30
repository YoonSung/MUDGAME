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


void CRoom::addPlayer(CPlayerCharacter& player)
{
	m_Player = &player;
	printf_s("m_PC in Room : %x\n", &player);
}

bool CRoom::addMonster(CMonster& monster)
{
	monster.SetPosition(MAP_SIZE-1, MAP_SIZE-1);

	bool isAdded = false;

	for ( int index = 0 ; index < MAX_MONSTER_NUM ; ++index )
	{
		if ( m_Monster[index] == NULL )
		{
			m_Monster[index] = &monster;
			isAdded = true;
			break;
		}
	}

	return isAdded;
}

CRoom::CRoom(void)
{
	memset(m_Monster, NULL, MAX_MONSTER_NUM);
	StartCatchMeIfYouCan();
}

void CRoom::StartCatchMeIfYouCan()
{

}

CRoom::~CRoom(void)
{
	//메모리 반환코드 필요하다.
}
