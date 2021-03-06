#include "stdafx.h"
#include "Monster.h"
#include "GameMap.h"
#include "Room.h"

//why this variable can't be member variable..? why error is occur?
//1>c:\users\yoonsung\documents\visual studio 2012\projects\cpp\mudgame\sud\monster.h(16): error C2143: 구문 오류 : ';'이(가) '*' 앞에 없습니다.
//1>c:\users\yoonsung\documents\visual studio 2012\projects\cpp\mudgame\sud\monster.h(16): error C4430: 형식 지정자가 없습니다. int로 가정합니다. 참고: C++에서는 기본 int를 지원하지 않습니다.
CGameMap* m_Map;
CRoom* m_Room;

CMonster::CMonster( int startX, int startY )
{
	m_Map = CGameMap::getInstancePtr();
	m_Room = CRoom::getInstancePtr();

	SetPosition (startX, startY);
}

//not allowed to call this function
CMonster::CMonster(void) {}


CMonster::~CMonster(void) {}

void CMonster::_Move ( DIRECTION dir, BOOL IsCallFromRoom )
{
	MapInfo* mInfo;
	
	if ( IsCallFromRoom )
		mInfo = m_Room->GetMapInfo( m_position.x, m_position.y );
	else
		mInfo = m_Map->GetMapInfo( m_position.x, m_position.y );
	
	
	//m_Map->deleteMopInMapInfo( m_position.x, m_position.y );
	//delete (mInfo->pMob);

	int moveTogo_X = m_position.x;
	int moveTogo_Y = m_position.y;


	switch ( dir )
	{
	case NONE:
		return;
	case DIR_UP:
		if ( moveTogo_Y == 0 )
			return;
		--moveTogo_Y;
		break;
	case DIR_DOWN:
		if ( moveTogo_Y == 9 )
			return;
		++moveTogo_Y;
		break;
	case DIR_LEFT:
		if ( moveTogo_X == 0 )
			return;
		--moveTogo_X;
		break;
	case DIR_RIGHT:
		if ( moveTogo_X == 9 )
			return;
		++moveTogo_X;
		break;
	}


	MapInfo* targetMapInfo;

	if ( IsCallFromRoom )
		targetMapInfo = m_Room->GetMapInfo( moveTogo_X, moveTogo_Y );
	else
		targetMapInfo = m_Map->GetMapInfo( moveTogo_X, moveTogo_Y );
	
	if ( targetMapInfo != nullptr )
	{
		if ( targetMapInfo->pMob != nullptr )
		{
			return;
		}
		else
		{
			mInfo->pMob = nullptr;
			targetMapInfo->pMob = this;
			SetPosition ( moveTogo_X, moveTogo_Y);
		}
	}
	//__super::Move( dir );
	//printf("position : %d, %d\n",m_position.x, m_position.y);
}

void CMonster::Move ( DIRECTION dir )
{
	_Move(dir, false);
}

void CMonster::MoveInRoom ( DIRECTION dir )
{
	_Move(dir, true);
}

//not allowed to call this function From extern, only access from constructor
void CMonster::SetPosition ( Position pos )
{
	__super::SetPosition( pos );
}

void CMonster::SetPosition (int x, int y)
{
	__super::SetPosition ( x, y );
}