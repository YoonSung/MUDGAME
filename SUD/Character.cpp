#include "stdafx.h"
#include "Character.h"
#include "GameMap.h"

//멤버 변수로 위치 만들고
//위치 출력하는 함수 만들고
//그리고 각각 키 입력받으면 위치 거시기

CCharacter::CCharacter(void)
{
	m_position.x = m_position.y = 0;
}


CCharacter::~CCharacter(void)
{
}

void CCharacter::SetPosition( int x, int y )
{
	Position position = { x, y };
	SetPosition( position );
}

void CCharacter::SetPosition( Position pos )
{
	m_position = pos;
}

void CCharacter::Move( DIRECTION dir )
{
	switch( dir )
	{
	case DIR_UP:
		m_position.y = __max( m_position.y - 1, 0 );
		break;

	case DIR_DOWN:
		m_position.y = __min( m_position.y + 1, MAP_SIZE - 1 );
		break;

	case DIR_LEFT:
		m_position.x = __max( m_position.x - 1, 0 );
		break;

	case DIR_RIGHT:
		m_position.x = __min( m_position.x + 1, MAP_SIZE - 1 );
		break;
	}

	//printf_s( "current pos = ( %d, %d )\n", m_position.x, m_position.y );
}
