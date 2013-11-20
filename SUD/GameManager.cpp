#include "stdafx.h"
#include "GameManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <conio.h>

#define VACANT_SPACE	"�� "
#define ENEMY_SPACE		"�� "
#define TARGET_SPACE	"�� "
#define PLAYER_SPACE	"�� "
#define NEWLINE			"\n\n"
#define CLEAR_MONITOR	"cls"

//��áڢ�

bool m_Flag = true;
CGameMap* h_Map = nullptr;
CPlayerCharacter* h_PC = nullptr;

unsigned int WINAPI ThreadProc( LPVOID lpParam );
void PrintExceptEnemy();
void PrintAllThing() ;
void _Print(char endXPos, char endYPos, std::string enemySpot);



CGameManager::CGameManager(void)
{
	m_PC = new CPlayerCharacter();
	m_Map = new CGameMap();
}

CGameManager::~CGameManager(void)
{
	delete m_PC;
}

void CGameManager::Init()
{
	//���ӿ� ���� ����κ��� �־�� �Ѵ�.
	//���̵�, ����� ��� �ҷ����� ���� ó���Ǿ�� �Ѵ�.
	printf_s ( "[Game Start!!]\n" );
	srand ( ( unsigned ) time ( NULL ) );

	CreateMobs();
	AutoMapDisplayON();
	m_PC->SetPosition( 0, 0 );
}

void CGameManager::Run()
{
	while(InputProc())
	{
		if ( CheckMissionClear() )
			EndMission();

		CheckCombatOccur();
	}
}

void CGameManager::Release()
{
	printf_s ( "[Game Over!!]\n" ) ;
}

bool CGameManager::InputProc()
{
	//Ű���� �Է��� �޾Ƽ� 
	//Ŀ�ǵ带 �Ľ��ϰ� �����Ѵ�.
	//esc�� �Է¹����� ����ȴ�
	
	
	char strInput = _getch(); 

	if ( strInput == 'w' || strInput == 'W')
		m_PC->Move(DIR_UP);
	if ( strInput == 's' || strInput == 'S')
		m_PC->Move ( DIR_DOWN ) ;
	if ( strInput == 'a' || strInput == 'A')
		m_PC->Move ( DIR_LEFT ) ;
	if ( strInput == 'd' || strInput == 'D')
		m_PC->Move ( DIR_RIGHT ) ;

	//ȭ��ǥ�� ���濹��. esc�� ȯ�漳�� �������� ���� �߰����� �ʿ�

	PrintExceptEnemy();
	return true;
}

//it is no use after now.
bool CGameManager::IsInput ( std::string inputValue, const char* compareValue )
{
	return ( strcmp ( inputValue.c_str(), compareValue ) == 0 );
}


void CGameManager::CheckCombatOccur()
{
	int x,y;

	x = m_PC->GetPositionX();
	y = m_PC->GetPositionY();

	MapInfo* currentMapInfo = m_Map->GetMapInfo(x, y);

	if ( currentMapInfo->pMob != nullptr )
		OccurCombat();
}


void CGameManager::CreateMobs()
{
	//�÷��̾� ������, ��ǥ�������� ���� ������ �ʵ��� ó���� �ʿ��ϴ�. 
	char buf [ 32 ] =  { 0, };

	int mobCount = ( MAP_SIZE * MAP_SIZE ) / 4 ;

	while ( mobCount > 0 )
	{
		int x = rand() % MAP_SIZE;
		int y = rand() % MAP_SIZE;

		MapInfo* pMapInfo = m_Map->GetMapInfo( x, y );

		if ( pMapInfo->pMob == nullptr )
		{
			pMapInfo->pMob = new CMonster();

			sprintf_s ( buf, "Mob %d" , mobCount );
			//pMapInfo->pMob->SetName(buf);

			--mobCount;
		}
	}

	//printf_s ( "<< Mob Create Complete! >>\n" );
}

void PrintAllThing() 
{
	_Print ( MAP_SIZE-1, MAP_SIZE-1, ENEMY_SPACE);
}

void PrintExceptEnemy()
{
	_Print ( MAP_SIZE-1, MAP_SIZE-1, VACANT_SPACE);
}

void _Print(char endXPos, char endYPos, std::string enemySpot)
{
	system ( CLEAR_MONITOR );
	std::string  buf;


	for ( int y = 0 ; y < MAP_SIZE ; ++y )
	{
		for ( int x = 0 ; x < MAP_SIZE ; ++x )
		{
			if ( h_PC->GetPositionY() == y && h_PC->GetPositionX() == x )
			{
				buf.append ( PLAYER_SPACE );
				continue;
			}

			if ( x == endXPos && y == endYPos )
			{
				buf.append ( TARGET_SPACE );
				continue;
			}

			MapInfo* pMapInfo = h_Map->GetMapInfo( x, y );

			if ( pMapInfo->pMob == nullptr )
				buf.append ( VACANT_SPACE );
			else
				buf.append ( enemySpot );
		}
		buf.append ( NEWLINE );
	}
	buf.append("\n\n");
	printf_s(buf.c_str());
}

unsigned int WINAPI ThreadProc( LPVOID lpParam )
{

	while ( m_Flag )
	{
		Sleep(500);
		PrintExceptEnemy();
		Sleep(3000);
		PrintAllThing();
	}
	return 0;
}

void CGameManager::AutoMapDisplayON()
{
	m_Flag = true;
	AutoMapDisplay();
}

void CGameManager::AutoMapDisPlayOFF()
{
	m_Flag = false;
	system ( CLEAR_MONITOR );
}

void CGameManager::AutoMapDisplay()
{
	DWORD dwThreadId;
	HANDLE hThread;

	h_Map = m_Map;
	h_PC = m_PC;

	hThread = ( HANDLE ) _beginthreadex( NULL, 0, ThreadProc, NULL, 0, ( unsigned int* )&dwThreadId );
	CloseHandle ( hThread );
}

void CGameManager::OccurCombat()
{
	AutoMapDisPlayOFF();
	PrintExceptEnemy();
	printf_s ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���� �������!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
	Sleep(3500);
	system ( CLEAR_MONITOR );

	while( InputProcInCombat() );

	EndCombat();
}

void CGameManager::EndCombat()
{
	AutoMapDisplayON();

	//������ ���� �ٲ� ��Ȳ���� ���⼭ ó��
}

bool CGameManager::InputProcInCombat()
{
	std::string strInput;


	printf_s( "����� �׽�Ʈ ���̴� '�̰��'�� �Է��Ͻø� ������ ����˴ϴ�.\n" );
	

	printf_s( "��� => ");
	getline(std::cin, strInput);

	//printf_s( "INPUT:%s\n",strInput.c_str() );


	if ( IsInput ( strInput, "q") )  {
		printf_s ("quit\n");
		//����ĥ �� ������ �г�Ƽ�� ũ�� �ۿ��� �� �ֵ���
		return false;
	} 
	else if ( IsInput ( strInput, "�̰��") )
	{
		printf_s ( "����� �̰���~ ���~~\n" );
		destroyMonster(m_PC->GetPositionX(), m_PC->GetPositionY());

		return false;
	}
	else
	{
		printf_s ( "������ ����� �������� �ʳ׿�. ��~��!!!\n" );
	}
	
	return true;
	//������ �Է��ϴ� ���°� �ƴ� gui���·� ���࿹��,
	//������ �ð��� ����Կ� ���� �ֺ����ֵ��� ������������ �̵��� Ȯ�� ����.
}

void CGameManager::destroyMonster(int x, int y)
{
	m_Map->GetMapInfo(x, y)->pMob = nullptr;
}

bool CGameManager::CheckMissionClear()
{
	int x,y;

	x = m_PC->GetPositionX();
	y = m_PC->GetPositionY();

	//���� map_size ������ ������ �̼�Ŭ���� ��Ҵ� ������ �ʿ�
	if ( x == MAP_SIZE-1 && y == MAP_SIZE-1 )
		return true;
	
	return false;
}

void CGameManager::EndMission()
{
	AutoMapDisPlayOFF();
	PrintExceptEnemy();
	printf_s ("�̼��� ����Ǿ����ϴ�.\n");
	Sleep(3500);
	printf_s ("���� �׽�Ʈ���̹Ƿ� ������ ���Ǹ� �� �� �־��.\n���α׷��� �����մϴ� ^^.");

	Sleep(2000);
	//���� �Ž��� �ö󰡼� InputProc() �Լ��� �ǵ��ư���. ������ ������ inputProc�� true���̴�. �̼����ῡ ���� ó���÷ο찡 �ʿ���.
	exit(1);
}
