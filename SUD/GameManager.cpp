#include "stdafx.h"
#include "GameManager.h"

#define SWAP( x, y, temp ) { (temp = x); (x = y); (y = temp );}

//일반변수
bool m_isAIMovementON = false;
CGameMap* m2_Map;
CRoom* m2_Room;
//일반함수
unsigned int WINAPI MonstersAIMove( LPVOID pc );

CGameManager::CGameManager(void)
{
	m_PC = new CPlayerCharacter();

	
	m_Map = CGameMap::getInstancePtr();
	m2_Map = m_Map;

	m_Room = CRoom::getInstancePtr();
	m2_Room = m_Room;

	m_Printer = new CPrinter(*m_PC, *m_Map);


	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_MAXIMIZE);
}

CGameManager::~CGameManager(void)
{
	delete m_PC;
	delete m_Printer;
	CGameMap::releaseInstance();
}

void CGameManager::Init()
{
	//게임에 대한 설명부분이 있어야 한다.
	//난이도, 저장된 목록 불러오기 등이 처리되어야 한다.
	printf_s ( "[Game Start!!]\n" );


	CreateMobs(); //플레이어 주변 일정면적이하에는 생성될 수 없도록 처리해야 한다.
	m_Printer->AutoMapDisplayON();
	AutoAIMovementON();
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

	m_Printer->PrintExceptEnemy();
	//화살표로 변경예정. esc로 환경설정 페이지에 대한 추가사항 필요
	return true;
}

//it is no use after now.
bool CGameManager::IsInput ( std::string inputValue, const char* compareValue )
{
	// agebreak : string을 사용하는 경우, strcmp를 사용하지 않아도 바로 비교 가능합니다. 
	//return ( strcmp ( inputValue.c_str(), compareValue ) == 0 );
	return (inputValue == compareValue);
}


void CGameManager::CheckCombatOccur()
{
	int x,y;

	x = m_PC->GetPositionX();
	y = m_PC->GetPositionY();

	MapInfo* currentMapInfo = m_Map->GetMapInfo(x, y);

	if ( currentMapInfo->pMob != nullptr )
		OccurCombat();
	EndCombat();
}


void CGameManager::CreateMobs()
{
	//플레이어 시작점, 목표지점에는 몹이 생기지 않도록 처리가 필요하다. 
	char buf [ 32 ] =  { 0, };

	int mobCount = 1;//( MAP_SIZE * MAP_SIZE ) / 4 ;
	srand ( ( unsigned ) time ( NULL ) );

	while ( mobCount > 0 )
	{
		int x = (double) rand() / (RAND_MAX) * MAP_SIZE;
		int y = (double) rand() / (RAND_MAX) * MAP_SIZE;

		//몬스터 출현지역에 대한 설정. 플레이어 주변 4x4위치에는 몹이 생성되지 않도록 하기 위하여
		if ( x < ( m_PC->GetPositionX()+3 ) && x > ( m_PC->GetPositionX()-3 ))
			continue;

		if ( y < ( m_PC->GetPositionX()+3 ) && y > ( m_PC->GetPositionX()-3 ))
			continue;

		MapInfo* pMapInfo = m_Map->GetMapInfo( x, y );

		if ( pMapInfo->pMob == nullptr )
		{
			//pMapInfo->pMob = new CMonster();
			pMapInfo->pMob = new CMonster( x, y );
			sprintf_s ( buf, "Mob %d" , mobCount );
			//pMapInfo->pMob->SetName(buf);

			--mobCount;
		}
	}

	//printf_s ( "<< Mob Create Complete! >>\n" );
}

void CGameManager::AutoAIMovementON()
{
	DWORD dwThreadId;
	HANDLE hThread;
	m_isAIMovementON = true;

	hThread = ( HANDLE ) _beginthreadex( NULL, 0, MonstersAIMove, m_PC, 0, ( unsigned int* )&dwThreadId );
	CloseHandle ( hThread );
};

void CGameManager::AutoAIMovementOFF()
{
	m_isAIMovementON = false;
}

unsigned int WINAPI MonstersAIMove( LPVOID PlayerCharacter )
{
	CPlayerCharacter* pc = reinterpret_cast<CPlayerCharacter*>(PlayerCharacter);
	// 	printf("position : x = %d, y = %d", pc->GetPositionX(), pc->GetPositionY());

	while ( m_isAIMovementON )
	{
		Sleep(6100);

		CMonster* monsters[MAP_SIZE*MAP_SIZE];
		memset(monsters, 0, MAP_SIZE*MAP_SIZE);

		int index = 0;

		for ( int x = 0 ; x < MAP_SIZE ; ++x )
		{
			for ( int y = 0 ; y < MAP_SIZE ; ++y )
			{
				MapInfo* pMapInfo = m2_Map->GetMapInfo( x, y );

				if ( pMapInfo->pMob != nullptr )
				{
					monsters[index] = pMapInfo->pMob;
					++index;
				}
			}
		}

		//index는 총 몬스터 갯수이다. 마지막에 ++로 넘어왔기 때문에 -1배열로 monster에 저장하고 있다.
		while ( index>=1 )
		{
			//최종적으로 이동할 위치를 담을 변수
			DIRECTION DirectionTogo_Result = NONE;

			CMonster* monster = monsters[index-1];

			if ( monster == nullptr)
				continue;

			float MovementWeight = 0.9f; //90% 가중치
			float MovementDeterminant = ((double) rand() / (RAND_MAX));

			if ( MovementWeight < MovementDeterminant )
			{
				DIRECTION possibleTogo[] = {NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
				DirectionTogo_Result = possibleTogo[(int)(((double) rand() / (RAND_MAX))*4)];
				
			}
			else
			{
				int MonsterPositionX = monster->GetPosition().x;
				int MonsterPositionY = monster->GetPosition().y;

				DIRECTION DirectionTogo_X = NONE;
				DIRECTION DirectionTogo_Y = NONE;

				int Between_Length_X = pc->GetPositionX() - MonsterPositionX;
				int Between_Length_Y = pc->GetPositionY() - MonsterPositionY;
				int Between_Length_Total = abs(Between_Length_X) + abs(Between_Length_Y);

				if ( Between_Length_Total == 0 )
				{
					printf_s("Room으로 좀비가 돌진합니다.\n");
					MapInfo* test = m2_Map->GetMapInfo(MonsterPositionX, MonsterPositionY);
					test->pMob = nullptr;
					monsters[index-1] = nullptr;
					m2_Room->addMonster(*monster);

					continue;
				}

				//플레이어가 몬스터보다 오른편에 존재.
				if ( Between_Length_X > 0 )
				{
					DirectionTogo_X = DIR_RIGHT;
				}
				else if ( Between_Length_X < 0 )
				{
					DirectionTogo_X = DIR_LEFT;
				}

				//플레이어가 몬스터보다 아래쪽에 있으면
				if ( Between_Length_Y > 0 )
				{
					DirectionTogo_Y = DIR_DOWN;
				}
				else if ( Between_Length_Y < 0 )
				{
					DirectionTogo_Y = DIR_UP; 
				}	

				//기본 할당수치 5+5 = 10
				int numberOfAllocation = DIR_ARRAY_MAXNUM / 2;
			
				//같은행, 또는 같은열에 없는경우.
				if ( DirectionTogo_X != NONE && DirectionTogo_Y != NONE )
				{
					int numTogo_X = DIR_ARRAY_MAXNUM / 4;
					int numTogo_Y = DIR_ARRAY_MAXNUM / 4;

					if ( Between_Length_Total <= 15 )
					{
						numberOfAllocation = 21 - Between_Length_Total;//21 - Between_Length_Total;
					}

					//할당 받은 개수가 짝수일 경우
					if ( numberOfAllocation % 2 == 0 )
					{
						//togo x,y 할당
						numTogo_X = numTogo_Y = numberOfAllocation / 2;
					}

					//할당 받은 개수가 홀수일 경우
					else
					{
						if ( Between_Length_X < Between_Length_Y )
						{
							//togo x,y 할당
							numTogo_X = numTogo_Y = numberOfAllocation / 2;

							//가까운쪽에 1을 더 더함
							++numTogo_X;
						}
						else
						{
							//togo x,y 할당
							numTogo_X = numTogo_Y = numberOfAllocation / 2;
							++numTogo_Y;
						}
					}

					DIRECTION dirArray[ DIR_ARRAY_MAXNUM ];

					int i = 0;
					while ( i < 20)
					{
						for ( int j = 0 ; j < numTogo_X ; ++j )
						{
							dirArray[i] = DirectionTogo_X;
							++i;
						}

						for ( int j = 0 ; j < numTogo_Y ; ++j )
						{
							dirArray[i] = DirectionTogo_Y;
							++i;
						}
					}

					//배열 셔플.
					srand ( ( unsigned ) time ( NULL ) );
					for ( int i = DIR_ARRAY_MAXNUM-1 ; i > 0 ; --i )
					{
						DIRECTION temp;
						int target =  rand()%i;
						SWAP ( dirArray[i], dirArray[target], temp );
					}
					DirectionTogo_Result = dirArray[ (int)((double)rand() / RAND_MAX * DIR_ARRAY_MAXNUM) ];
			
					/*//test code
					printf_s("DIR_UP    : 1\n");
					printf_s("DIR_Down  : 2\n");
					printf_s("DIR_LEFT  : 3\n");
					printf_s("DIR_RIGHT : 4\n\n");
					
					for ( int i = 0 ; i < DIR_ARRAY_MAXNUM ; ++i )
						printf_s("arr[i] = %d\n", dirArray[i]);
			
					printf_s("\n\nPC_pos x : %d , PC_pos y : %d\n",pc->GetPositionX(), pc->GetPositionY());
					printf_s("M_pos x : %d , M_pos y : %d\n",MonsterPositionX, MonsterPositionY);
					
					printf_s("Dir_X : %d, Dir_Y : %d\n", DirectionTogo_X, DirectionTogo_Y);
					printf_s("togo_X : %d, togo_Y : %d\n", numTogo_X, numTogo_Y);
					*/

				} else {
					if ( DirectionTogo_X == NONE )
						DirectionTogo_Result = DirectionTogo_Y;
					else 
						DirectionTogo_Result = DirectionTogo_X;
				}
			}
			printf_s("MovementWeight : %f\n", MovementWeight );
			printf_s("MovementDeterminant : %f\n", MovementDeterminant );


			printf_s("DirectionTogo_Result : %d\n", DirectionTogo_Result );
			monster->Move( DirectionTogo_Result );
			
			Sleep(50); //몬스터별로 이동하는 시간을 다르게 하고 싶어서 넣은 sleep문
			--index;
		}//맵 전체의 몬스터들을 한번씩 이동시키는  while문
	} //주기적으로 AIMovement를 시행하는 while 문, movementOn 이라는 flag를 통해서 수행된다.
	return 0;
}

void CGameManager::OccurCombat()
{
	m_Printer->CombatModeON();
	m_Room->addPlayer(*m_PC);


	//
	//	PrintExceptEnemy();
	m_Printer->AddLogBuffer("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!좀비를 만났어요!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	//	printf_s ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!좀비를 만났어요!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
	// 	Sleep(3000);
	// 	//Room 클래스 생성, 현재 플레이어 정보 및 좀비 정보를 전달.
	// 	//화면 오른편에 전투모습 전시, 다른 좀비들이 다가오는 모습도 그대로 노출.
	// 	//상단의 슬립과 맵플레이 함수 없애기
	// 	system ( CLEAR_MONITOR );


	while( InputProcInCombat() );
}

void CGameManager::EndCombat()
{
	//m_Printer->AutoMapDisplayON();

	//전투로 인해 바뀐 상황들을 여기서 처리
	m_Printer->CombatModeOFF();
}

bool CGameManager::InputProcInCombat()
{
	std::string strInput;


	m_Printer->AddLogBuffer("현재는 테스트 중이니 '이겼다'를 입력하시면 전투가 종료됩니다.\n" );


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
	delete m_Map->GetMapInfo(x, y)->pMob;
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
	m_Printer->AutoMapDisPlayOFF();
	//	PrintExceptEnemy();
	printf_s ("미션이 종료되었습니다.\n");
	printf_s ("현재 테스트중이므로 게임은 한판만 할 수 있어요.\n프로그램을 종료합니다 ^^.");

	Sleep(2000);
	//본래 거슬러 올라가서 InputProc() 함수로 되돌아간다. 하지만 여전히 inputProc은 true값이다. 미션종료에 대한 처리플로우가 필요함.
	exit(1);
}
