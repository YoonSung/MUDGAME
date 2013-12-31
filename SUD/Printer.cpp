#include "stdafx.h"
#include "Printer.h"
#include <sstream>

#define VACANT_SPACE	"□"
#define ENEMY_SPACE		"※"
#define TARGET_SPACE	"♥"
#define PLAYER_SPACE	"★"
#define NEWLINE_TWICE	"\n\n" 
#define CLEAR_MONITOR	"cls"
#define NEWLINE			"\n"
#define TAP				"\t\t\t" 
#define PRINT_INTERVAL		500
#define TIME_BETWEEN_PRINT 1000

#define PLAYER_SPACE_IN_ROOM	"□"
#define PLAYER_WITH_TARGET		"▣"
#define TARGET_SPACE_IN_ROOM	"※" 
#define VACANT_SPACE_IN_ROOM	"  "
#define MAX_LOG_LENGTH		70

//static 변수 선언
CPrinter* CPrinter::instance;

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
std::string convertString(int value);

void CPrinter::AddLogBuffer(std::string logString)
{
	for ( int i = 1 ; i < MAX_LOG_NUM; ++i )
	{
		m_LogBuffer[i-1] = m_LogBuffer[i];
	}

	m_LogBuffer[MAX_LOG_NUM - 1] = logString;
}


std::string getStatusView()
{
	std::string view;

	CPlayerCharacter* target;

	if ( m_isCombatOccur )
	{
		target = _m_PC_Room;
	}
	else
	{
		target = _m_PC;
	}
	

	view.append("┃");
	view.append("레벨 : ");
	view.append( convertString(target->GetLevel()) );
	view.append("┃");
	view.append("체력 : ");
	view.append( convertString(target->GetEnergy()) );
	view.append("┃");
	view.append("경험치 : ");
	view.append( convertString(target->GetExperience()) );
	view.append("┃");
	view.append("누적사살횟수 : ");
	view.append( convertString(target->GetTotalKillingNum()) );

	view.append(NEWLINE);

	return view;
}

std::string convertString(int value)
{
	std::stringstream out;
	out << value;

	return out.str();
}

std::string getLogView()
{
	std::string view;

	view.append("┏");

	for(int i = 0; i < MAX_LOG_LENGTH-35; ++i) {
		view.append("━");
	}
		view.append("┓");
	view.append( NEWLINE );

	for ( int i = 0 ; i < MAX_LOG_NUM ; ++i )
	{
		view.append("┃");

		int size = m_LogBuffer[i].length();

		std::string temp;

		for ( int j = 0 ; j < MAX_LOG_LENGTH-size ; ++j)
		{
			temp.append(" ");
		}

		view.append(m_LogBuffer[i]);
		view.append(temp);
		view.append("┃");
		view.append( NEWLINE );
	}

	view.append("┗");
	for(int i = 0; i < MAX_LOG_LENGTH-35; ++i) {
		view.append("━");
	}
	view.append("┛");

	return view;
}

std::string getMapView(std::string enemySymbol)
{

	std::string view;

	view.append("┏");
	for(int i = 0; i < MAP_SIZE; ++i) {
		view.append("━");
	}
	view.append("┓");

	if ( m_isCombatOccur )
	{
		view.append( TAP );
		view.append("┏");
		for(int i = 0; i < MAP_SIZE; ++i) {
			view.append("━");
		}
		view.append("┓");
	}

	view.append(NEWLINE);

	for ( int y = 0 ; y < MAP_SIZE ; ++y )
	{ 
		view.append("┃");
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
		view.append("┃");
		//만약 전투가 시작되면 오른쪽 맵도 보여준다.
		if ( m_isCombatOccur )
		{
			view.append( TAP );
			view.append("┃");
			for ( int x = 0 ; x < MAP_SIZE ; ++x )
			{

				MapInfo* pRoomMapInfo = CRoom::getInstancePtr()->GetMapInfo( x, y );

				if ( _m_PC_Room->GetPositionY() == y && _m_PC_Room->GetPositionX() == x )
				{
					if ( pRoomMapInfo->pMob != nullptr )
					{
						view.append ( PLAYER_WITH_TARGET );
					}
					else
					{
						view.append ( PLAYER_SPACE_IN_ROOM );
					}
					continue;
				}
				if ( pRoomMapInfo->pMob == nullptr )
					view.append ( VACANT_SPACE_IN_ROOM );
				else
					view.append ( enemySymbol );
			}
			view.append("┃");
		}
		view.append ( NEWLINE );
	}

	view.append("┗");
	for(int i = 0; i < MAP_SIZE; ++i) {
		view.append("━");
	}
	view.append("┛");

	if ( m_isCombatOccur )
	{
		view.append( TAP );
		view.append("┗");
		for(int i = 0; i < MAP_SIZE; ++i) {
			view.append("━");
		}
		view.append("┛");
	}


	view.append(NEWLINE_TWICE);

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
	view.append(getStatusView());
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

CPrinter::CPrinter(){}

CPrinter* CPrinter::getInstancePtr()
{
	if ( instance == nullptr )
	{
		instance = new CPrinter();
	}
	return instance;
}
void CPrinter::releaseInstance()
{
	delete instance;
}

void CPrinter::init(CPlayerCharacter& PC)
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
