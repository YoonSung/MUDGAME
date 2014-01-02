#include "stdafx.h"
#include "Room.h"
#include "Monster.h"
#include "Printer.h"

CRoom* CRoom::instance;

CRoom* CRoom::getInstancePtr()
{
	if ( instance == nullptr )
	{
		instance = new CRoom();
	}
	return instance;
}

void CRoom::Init()
{
	delete instance;
	instance = nullptr;
}

void CRoom::releaseInstance()
{
	delete instance;
}

CPlayerCharacter* CRoom::getPlayer()
{
	return m_MockPlayer;
}

bool CRoom::addMonster(CMonster& monster)
{	

	bool isAdded = false;

	MapInfo* tempMapInfo = GetMapInfo( monster.GetPosition().x, monster.GetPosition().y);

 	for ( int index = 0 ; index < MAX_MONSTER_NUM ; ++index )
 	{
 		if ( tempMapInfo->pMob == nullptr )
 		{
 			tempMapInfo->pMob = &monster;
 			isAdded = true;
 			++m_CurrentMonsterNumber;
 			break;
 		}
 	}

	return isAdded;
}

CRoom::CRoom(void)
{
	m_MockPlayer = new CPlayerCharacter();
	m_MockPlayer->SetPosition(0,0);
	m_CurrentMonsterNumber = 0;
}

CRoom::~CRoom(void)
{
	delete m_MockPlayer;
}

void CRoom::CheckZombieCapture()
{
	int x = m_MockPlayer->GetPosition().x;
	int y = m_MockPlayer->GetPosition().y;
	MapInfo* mapInfo = GetMapInfo(x, y);

	CPrinter* printer = CPrinter::getInstancePtr();


	if (  mapInfo->pMob != nullptr )
	{

		m_MockPlayer->KillingZombie( (mapInfo->pMob)->GetExperience() );
		printer->AddLogBuffer("좀비를 잡았습니다!!");
		delete mapInfo->pMob;
		mapInfo->pMob = nullptr;
		m_CurrentMonsterNumber--;
	}
	else
	{
		printer->AddLogBuffer("힘 낭비를 하지마세요! 좀비가 없는 지역입니다");
	}
}

int CRoom::getMonsterNumberInRoom()
{
	return m_CurrentMonsterNumber;
}

void CRoom::AttackPlayer(int damage)
{
	m_MockPlayer->AttackFromEnemy(damage);
}

bool CRoom::IsPlayerDead()
{
	return m_MockPlayer->IsPlayerDead();
}

void CRoom::SynchronizePlayer( int level, int energy, int experience, int killingNum )
{
	m_MockPlayer->SetLevel( level );
	m_MockPlayer->SetEnergy( energy );
	m_MockPlayer->SetExperience( experience );
	m_MockPlayer->SetTotalKillingNum( killingNum );
}
