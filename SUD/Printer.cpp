#include "stdafx.h"
#include "Printer.h"

#define VACANT_SPACE	"□ "
#define ENEMY_SPACE		"▣ "
#define TARGET_SPACE	"♥ "
#define PLAYER_SPACE	"★ "
#define NEWLINE			"\n\n"
#define CLEAR_MONITOR	"cls"

bool m_Flag = true;
CPlayerCharacter* _m_PC;
CGameMap* _m_Map;	
bool m_isCombatOccur;
std::string m_LogBuffer[10];

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

			if ( pMapInfo->pMob == nullptr )
				view.append ( VACANT_SPACE );
			else
				view.append ( enemySymbol );
		}
		view.append ( NEWLINE );
	}
	view.append( NEWLINE );
	view.append( NEWLINE );

	return view;
}

void CPrinter::ShowScreen()
{

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
	system ( CLEAR_MONITOR );
	view.append(getMapView(enemySymbol));
//	view.append(getLogView());

	printf(view.c_str());
}

// agebreak : 화면 출력에 스레드를 사용한건 좋은 접근입니다. ^^
unsigned int WINAPI ThreadProc( LPVOID lpParam )
{

	while ( m_Flag )
	{
		Sleep(500);

		if ( !m_isCombatOccur )
		{
			_PrintExceptEnemy();
			Sleep(3000);
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

CPrinter::CPrinter(CPlayerCharacter& PC, CGameMap& MAP)
{
	_m_PC = &PC;
	_m_Map = &MAP;
}


CPrinter::~CPrinter(void)
{
}
