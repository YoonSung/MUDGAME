#include "stdafx.h"
#include "Room.h"


CRoom::CRoom(CMonster* monster, CPlayerCharacter* player)
{
	m_Player = player;
	memset(m_Monster, 0, MAX_MONSTER_NUM);

	for ( int index = 0 ; index < MAX_MONSTER_NUM ; ++index )
	{
		if ( m_Monster[index] == NULL )
		{
			m_Monster[index] = monster;
			break;
		}
	}

	StartCatchMeIfYouCan();
}

void CRoom::StartCatchMeIfYouCan()
{

}






CRoom::~CRoom(void)
{
}
