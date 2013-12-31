#include "stdafx.h"
#include "Printer.h"

#define VACANT_SPACE	"□ "
#define ENEMY_SPACE		"▣ "
#define TARGET_SPACE	"♥ "
#define PLAYER_SPACE	"★ "
#define NEWLINE_TWICE	"\n\n"
#define CLEAR_MONITOR	"cls"
#define NEWLINE			"\n"
#define TAP				"\t\t\t"
#define PRINT_INTERVAL		500
#define TIME_BETWEEN_PRINT 1000

//print 일반함수에서 다루어야 하기 때문에 일반변수로 선언.
bool m_Flag = true;
CPlayerCharacter* _m_PC;
CPlayerCharacter* _m_PC_Room;
CGameMap* _m_Map;
bool m_isCombatOccur;
std::string m_LogBuffer[10];
CRoom* _m_Room;


//내부함수 선언
void _PrintAllThing();
void _PrintExceptEnemy();
void _Print(std::string enemySymbol);
std::string getMapView(std::string enemySymbol);


void CPrinter::AddLogBuffer(std::string logString)
{
	for ( int i = 1 ; i < MAX_LOG_NUM; ++i )
	{
		m_LogBuffer[i-1] = m_LogBuffer[i];
	}

	m_LogBuffer[MAX_LOG_NUM - 1] = logString;
}

std::string getLogView()
{
	std::string view;

	for ( int i = 0 ; i < MAX_LOG_NUM ; ++i )
	{
		view.append(m_LogBuffer[i]);
		view.append( NEWLINE );
	}

	return view;
}

std::string getMapView(std::string enemySymbol)
{

	std::string view;

	for ( int y = 0 ; y < MAP_SIZE ; ++y )
	{
		for ( int x = 0 ; x < MAP_SIZE ; ++x )
		{
			if ( _m_PC->GetPositionY() == y && _m_PC->GetPositionX() == x )
			{
				view.append ( PLAYER_SPACE );
				continue;
			}

			if ( x == MAP_SIZE-1 && y == MAP_SIZE-1 )
			{
				view.append ( TARGET_SPACE );
				continue;
			}

			MapInfo* pMapInfo = _m_Map->GetMapInfo( x, y );

			//기존의 왼쪽맵
			if ( pMapInfo->pMob == nullptr )
				view.append ( VACANT_SPACE );
			else
				view.append ( enemySymbol );
		}
		
		//만약 전투가 시작되면 오른쪽 맵도 보여준다.
		if ( m_isCombatOccur )
		{
			view.append( TAP );

			for ( int x = 0 ; x < MAP_SIZE ; ++x )
			{
				if ( _m_PC_Room->GetPositionY() == y && _m_PC_Room->GetPositionX() == x )
				{
					view.append ( PLAYER_SPACE );
					continue;
				}

				MapInfo* pRoomMapInfo = CRoom::getInstancePtr()->GetMapInfo( x, y );

				if ( pRoomMapInfo->pMob == nullptr )
					view.append ( VACANT_SPACE );
				else
					view.append ( enemySymbol );
			}
		}
		view.append ( NEWLINE_TWICE );
	}

	return view;
}

void CPrinter::CombatModeON()
{
	m_isCombatOccur = true;
}

void CPrinter::CombatModeOFF()
{
	m_isCombatOccur = false;
}

void CPrinter::PrintExceptEnemy()
{
	_PrintExceptEnemy();
}

void CPrinter::PrintAllThing()
{
	_PrintAllThing();
}

void _PrintAllThing() 
{
	_Print ( ENEMY_SPACE );
}

void _PrintExceptEnemy()
{
	_Print ( VACANT_SPACE );
}

void _Print(std::string enemySymbol)
{
	std::string view;
	view.append(getMapView(enemySymbol));
	view.append(getLogView());
	system ( CLEAR_MONITOR );
	printf(view.c_str());
}

// agebreak : 화면 출력에 스레드를 사용한건 좋은 접근입니다. ^^
unsigned int WINAPI ThreadProc( LPVOID lpParam )
{

	while ( m_Flag )
	{
		Sleep(PRINT_INTERVAL);

		if ( !m_isCombatOccur )
		{
			_PrintExceptEnemy();
			Sleep(TIME_BETWEEN_PRINT);
		}

		_PrintAllThing();
	}
	return 0;
}

void CPrinter::AutoMapDisplayON()
{
	m_Flag = true;
	
	DWORD dwThreadId;
	HANDLE hThread;

	hThread = ( HANDLE ) _beginthreadex( NULL, 0, ThreadProc, NULL, 0, ( unsigned int* )&dwThreadId );
	CloseHandle ( hThread );
}

void CPrinter::AutoMapDisPlayOFF()
{
	m_Flag = false;
	system ( CLEAR_MONITOR );
}

CPrinter::CPrinter(CPlayerCharacter& PC)
{
	_m_PC = &PC;
	_m_Map = CGameMap::getInstancePtr();
	_m_Room = CRoom::getInstancePtr();
	_m_PC_Room = _m_Room->getPlayer();


	AddLogBuffer("게임을 시작합니다.");
	AddLogBuffer("몬스터들을 잘 피해서 목적지까지 이동하세요.");
	AddLogBuffer("몬스터를 만나면, 해당 좌표의 공간에서 전투가 이루어집니다.");
	AddLogBuffer("전투가 시작되면, 주변몹들이 플레이어의 위치를 알고 가까이 다가오니");
	AddLogBuffer("빨리 전투를 끝내야 합니다!!!");
	AddLogBuffer(" ");
	AddLogBuffer("부디.... 잘 살아남으시기 바랍니다.");
}


CPrinter::~CPrinter(void)
{
}
