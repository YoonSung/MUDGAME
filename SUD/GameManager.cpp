#include "stdafx.h"
#include "GameManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <conio.h>

#define VACANT_SPACE	"□ "
#define ENEMY_SPACE		"▣ "
#define TARGET_SPACE	"♥ "
#define PLAYER_SPACE	"★ "
#define NEWLINE			"\n\n"
#define CLEAR_MONITOR	"cls"

//□▣★♥

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
	//게임에 대한 설명부분이 있어야 한다.
	//난이도, 저장된 목록 불러오기 등이 처리되어야 한다.
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
	//키보드 입력을 받아서 
	//커맨드를 파싱하고 실행한다.
	//esc를 입력받으면 종료된다
	
	
	char strInput = _getch(); 

	if ( strInput == 'w' || strInput == 'W')
		m_PC->Move(DIR_UP);
	if ( strInput == 's' || strInput == 'S')
		m_PC->Move ( DIR_DOWN ) ;
	if ( strInput == 'a' || strInput == 'A')
		m_PC->Move ( DIR_LEFT ) ;
	if ( strInput == 'd' || strInput == 'D')
		m_PC->Move ( DIR_RIGHT ) ;

	//화살표로 변경예정. esc로 환경설정 페이지에 대한 추가사항 필요

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
	//플레이어 시작점, 목표지점에는 몹이 생기지 않도록 처리가 필요하다. 
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
	printf_s ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!좀비를 만났어요!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
	Sleep(3500);
	system ( CLEAR_MONITOR );

	while( InputProcInCombat() );

	EndCombat();
}

void CGameManager::EndCombat()
{
	AutoMapDisplayON();

	//전투로 인해 바뀐 상황들을 여기서 처리
}

bool CGameManager::InputProcInCombat()
{
	std::string strInput;


	printf_s( "현재는 테스트 중이니 '이겼다'를 입력하시면 전투가 종료됩니다.\n" );
	

	printf_s( "명령 => ");
	getline(std::cin, strInput);

	//printf_s( "INPUT:%s\n",strInput.c_str() );


	if ( IsInput ( strInput, "q") )  {
		printf_s ("quit\n");
		//도망칠 수 있으나 패널티가 크게 작용할 수 있도록
		return false;
	} 
	else if ( IsInput ( strInput, "이겼다") )
	{
		printf_s ( "당신이 이겼어요~ 우와~~\n" );
		destroyMonster(m_PC->GetPositionX(), m_PC->GetPositionY());

		return false;
	}
	else
	{
		printf_s ( "아직은 명령이 지원되지 않네요. 미~안!!!\n" );
	}
	
	return true;
	//전투는 입력하는 형태가 아닌 gui형태로 진행예정,
	//전투시 시간이 경과함에 따라서 주변유닛들이 전투지역으로 이동할 확률 증가.
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

	//현재 map_size 끝으로 정해진 미션클리어 장소는 변경이 필요
	if ( x == MAP_SIZE-1 && y == MAP_SIZE-1 )
		return true;
	
	return false;
}

void CGameManager::EndMission()
{
	AutoMapDisPlayOFF();
	PrintExceptEnemy();
	printf_s ("미션이 종료되었습니다.\n");
	Sleep(3500);
	printf_s ("현재 테스트중이므로 게임은 한판만 할 수 있어요.\n프로그램을 종료합니다 ^^.");

	Sleep(2000);
	//본래 거슬러 올라가서 InputProc() 함수로 되돌아간다. 하지만 여전히 inputProc은 true값이다. 미션종료에 대한 처리플로우가 필요함.
	exit(1);
}
