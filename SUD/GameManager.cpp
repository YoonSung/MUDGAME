#include "stdafx.h"
#include "GameManager.h"

#define SWAP( x, y, temp ) { (temp = x); (x = y); (y = temp );}
#define MOVE_INTERVAL_IN_ROOM	50
#define MOVE_INTERVAL			2100

//일반변수
bool m_IsAIMovementON = false;
bool m_IsCombatOccur = false;

CPlayerCharacter* m2_PC;
CGameMap* m2_Map;
CRoom* m2_Room;
CPrinter* m2_Printer;

//일반함수
void _MonsterAIMove( CPlayerCharacter* pc );
void _MonsterAIMoveInRoom(CPlayerCharacter* pc);
unsigned int WINAPI MonstersAIMove( LPVOID PlayerCharacter );

CGameManager::CGameManager(void)
{
	m_PC = new CPlayerCharacter();
	m2_PC = m_PC;
	
	m_Map = CGameMap::getInstancePtr();
	m2_Map = m_Map;

	m_Room = CRoom::getInstancePtr();
	m2_Room = m_Room;

	m_Printer = new CPrinter(*m_PC);
	m2_Printer = m_Printer;

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);//SW_MAXIMIZE);
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
	m_IsAIMovementON = true;

	hThread = ( HANDLE ) _beginthreadex( NULL, 0, MonstersAIMove, m_PC, 0, ( unsigned int* )&dwThreadId );
	CloseHandle ( hThread );
};

void CGameManager::AutoAIMovementOFF()
{
	m_IsAIMovementON = false;
}

unsigned int WINAPI MonstersAIMove( LPVOID PlayerCharacter )
{
	CPlayerCharacter* pc = reinterpret_cast<CPlayerCharacter*>(PlayerCharacter);
	// 	printf("position : x = %d, y = %d", pc->GetPositionX(), pc->GetPositionY());

	if ( m_IsCombatOccur )
	{
		while ( m_IsCombatOccur )
		{
			_MonsterAIMoveInRoom(pc);
		}
	}
	else
	{
		while ( m_IsAIMovementON )
		{
			_MonsterAIMove(pc);
		} //주기적으로 AIMovement를 시행하는 while 문, movementOn 이라는 flag를 통해서 수행된다.
	}
	
	return 0;
}

void _MonsterAIMoveInRoom(CPlayerCharacter* pc)
{
	Sleep(MOVE_INTERVAL_IN_ROOM);

	CMonster* monsters[MAP_SIZE*MAP_SIZE];
	memset(monsters, 0, MAP_SIZE*MAP_SIZE);

	int index = 0;

	for ( int x = 0 ; x < MAP_SIZE ; ++x )
	{
		for ( int y = 0 ; y < MAP_SIZE ; ++y )
		{
			MapInfo* pMapInfo = m2_Room->GetMapInfo( x, y );

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
		int MonsterPositionX = monster->GetPosition().x;
		int MonsterPositionY = monster->GetPosition().y;

		DIRECTION DirectionTogo_X = NONE;
		DIRECTION DirectionTogo_Y = NONE;
		DIRECTION DirectionTogo_Reverse_X = NONE;
		DIRECTION DirectionTogo_Reverse_Y = NONE;

		int Between_Length_X = pc->GetPositionX() - MonsterPositionX;
		int Between_Length_Y = pc->GetPositionY() - MonsterPositionY;
		int Between_Length_Total = abs(Between_Length_X) + abs(Between_Length_Y);

		if ( MovementWeight < MovementDeterminant || Between_Length_Total > 8)
		{
			DIRECTION possibleTogo[] = {NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
			DirectionTogo_Result = possibleTogo[(int)(((double) rand() / (RAND_MAX))*4)];	
		}
		else
		{
			//플레이어가 몬스터보다 오른편에 존재.
			if ( Between_Length_X > 0 )
			{
				//피해야 하므로 반대방향을 설정한다. map에서 했던 것의 반대.
				DirectionTogo_X = DIR_LEFT;
				DirectionTogo_Reverse_X = DIR_RIGHT;
			}
			else if ( Between_Length_X < 0 )
			{
				DirectionTogo_X = DIR_RIGHT;
				DirectionTogo_Reverse_X = DIR_LEFT;
			}

			//플레이어가 몬스터보다 아래쪽에 있으면
			if ( Between_Length_Y > 0 )
			{
				DirectionTogo_Y = DIR_UP;
				DirectionTogo_Reverse_X = DIR_DOWN;
			}
			else if ( Between_Length_Y < 0 )
			{
				DirectionTogo_Y = DIR_DOWN; 
				DirectionTogo_Reverse_X = DIR_UP;
			}	
			
			//같은행, 또는 같은열일 경우	
			if ( Between_Length_X == 0 || Between_Length_Y == 0 )
			{
// 				DIRECTION possibleTogo[2];
				
				if ( Between_Length_X == 0 )
				{
					( MonsterPositionX < 5 ) ? DirectionTogo_Result = DIR_RIGHT : DirectionTogo_Result = DIR_LEFT;

//					DirectionTogo_Result = DirectionTogo_Y;
// 					possibleTogo[0] = DirectionTogo_Y;
// 					possibleTogo[1] = DirectionTogo_Reverse_Y;
				}	
				else
				{
					( MonsterPositionY < 5 ) ? DirectionTogo_Result = DIR_DOWN : DirectionTogo_Result = DIR_UP;

//					DirectionTogo_Result = DirectionTogo_X;
// 					possibleTogo[0] = DirectionTogo_X;
// 					possibleTogo[1] = DirectionTogo_Reverse_X;
				}

				//DirectionTogo_Result = possibleTogo[(int)(((double) rand() / (RAND_MAX))*2)];	
				
			}
			else
			{
				DIRECTION possibleTogo[2] = { DirectionTogo_X, DirectionTogo_Y};
				

				if ( ( MonsterPositionX == 1 || MonsterPositionX == 9 ) && MonsterPositionY != 9 )
				{
					possibleTogo[0] = DirectionTogo_Reverse_X;
					possibleTogo[1] = DirectionTogo_Y;
				}
				else if ( MonsterPositionX != 9 && ( MonsterPositionY == 1 || MonsterPositionY == 9 ))
				{
					possibleTogo[0] = DirectionTogo_X;
					possibleTogo[1] = DirectionTogo_Reverse_Y; 
				}
				else if (  ( MonsterPositionX == 1 && MonsterPositionX == 9 ) == true && ( MonsterPositionY == 1 && MonsterPositionY == 9 ) == true )
				{
					if ( Between_Length_X > Between_Length_Y )
						possibleTogo[0] = possibleTogo[1] = DirectionTogo_Reverse_X;
					else
						possibleTogo[0] = possibleTogo[1] = DirectionTogo_Reverse_Y;
				}

				DirectionTogo_Result = possibleTogo[(int)(((double) rand() / (RAND_MAX))*2)];	
			}
		}
		/*
		//test
		printf_s("DIR_UP    : 1\n");
		printf_s("DIR_Down  : 2\n");
		printf_s("DIR_LEFT  : 3\n");
		printf_s("DIR_RIGHT : 4\n\n");

		printf_s("\n\nPC_pos x : %d , PC_pos y : %d\n",pc->GetPositionX(), pc->GetPositionY());
		printf_s("M_pos x : %d , M_pos y : %d\n",MonsterPositionX, MonsterPositionY);
		printf_s("Dir_X : %d, Dir_Y : %d\n", DirectionTogo_X, DirectionTogo_Y);
				

		printf_s("MovementWeight : %f\n", MovementWeight );
		printf_s("MovementDeterminant : %f\n", MovementDeterminant );


		printf_s("DirectionTogo_Result : %d\n", DirectionTogo_Result );
		*/
		monster->MoveInRoom( DirectionTogo_Result );
		m2_Printer->PrintAllThing();

		Sleep(10); //몬스터별로 이동하는 시간을 다르게 하고 싶어서 넣은 sleep문
		--index; //while문 첫번째로 돌아가기 전 몬스터 배열에서의 다음 몬스터를 가리키기 위한 인덱스
	}//맵 전체의 몬스터들을 한번씩 이동시키는  while문
}

void _MonsterAIMove(CPlayerCharacter* pc)
{
	Sleep(MOVE_INTERVAL);

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
				
				//좀비가 플레이어에게 이동하는 경우이므로 멘트를 아래와 같이 한다.
				m2_Printer->AddLogBuffer("좀비가 돌진해옵니다.\n");

				//이미 전투가 진행중이 아닐경우, 이미 전투중일때 주변 몬스터가 접근했을때 이 루프문을 무시하고 통과하게 된다. 즉, 최초 몬스터를 만날때만 실행된다.
				if ( !m_IsCombatOccur )
				{
					m2_Printer->AddLogBuffer("좀비와의 전투가 시작됩니다.");
					m2_Printer->AddLogBuffer("악랄한 좀비는 원거리에서 공격해옵니다.");
					m2_Printer->AddLogBuffer("좀비를 잡지 못하면 에너지가 계속 감소됩니다...!");
					m2_Printer->AddLogBuffer("키를 입력하시면 Room으로 입장합니다.");
				}

				m_IsCombatOccur = true;

				

				MapInfo* mapInfo = m2_Map->GetMapInfo(MonsterPositionX, MonsterPositionY);
					
				//기존의 맵에서 좀비 삭제
				mapInfo->pMob = nullptr;
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
		/*
		//test
		printf_s("MovementWeight : %f\n", MovementWeight );
		printf_s("MovementDeterminant : %f\n", MovementDeterminant );


		printf_s("DirectionTogo_Result : %d\n", DirectionTogo_Result );
		*/
		monster->Move( DirectionTogo_Result );
			
		Sleep(100); //몬스터별로 이동하는 시간을 다르게 하고 싶어서 넣은 sleep문
		--index; //while문 첫번째로 돌아가기 전 몬스터 배열에서의 다음 몬스터를 가리키기 위한 인덱스
	}//맵 전체의 몬스터들을 한번씩 이동시키는  while문
}

void CGameManager::CheckCombatOccur()
{
	int x,y;

	x = m_PC->GetPositionX();
	y = m_PC->GetPositionY();

	MapInfo* currentMapInfo = m_Map->GetMapInfo(x, y);


	if ( currentMapInfo->pMob != nullptr || m_IsCombatOccur)
	{
		if ( !m_IsCombatOccur )
		{
			m_IsCombatOccur = true;
			m_Printer->AddLogBuffer("이동한 지점에 좀비가 있네요!");
			m_Printer->AddLogBuffer("전투가 곧 시작됩니다.");

			//기존의 맵에서 좀비 삭제

			CMonster monster = *(currentMapInfo->pMob);
			printf_s("CurrentMapinfo   inGm  : %x\n", (currentMapInfo->pMob));
			printf_s("monster in GameManager : %x\n", &monster);
			m_Room->addMonster(monster);
			currentMapInfo->pMob = nullptr;

			Sleep(2100);
		}

		OccurCombat();
		EndCombat();
	}
}


void CGameManager::OccurCombat()
{
	m_Printer->CombatModeON();
	
	//	PrintExceptEnemy();

	//	printf_s ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!좀비를 만났어요!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
	// 	Sleep(3000);
	// 	//Room 클래스 생성, 현재 플레이어 정보 및 좀비 정보를 전달.
	// 	//화면 오른편에 전투모습 전시, 다른 좀비들이 다가오는 모습도 그대로 노출.
	// 	//상단의 슬립과 맵플레이 함수 없애기
	// 	system ( CLEAR_MONITOR );
	CPlayerCharacter* m_PC_Room = m_Room->getPlayer();

	DWORD dwThreadId;
	HANDLE hThread;

	hThread = ( HANDLE ) _beginthreadex( NULL, 0, MonstersAIMove, m_PC_Room, 0, ( unsigned int* )&dwThreadId );
	CloseHandle ( hThread );


	while( InputProcInCombat(m_PC_Room) );
}

void CGameManager::EndCombat()
{
	//m_Printer->AutoMapDisplayON();

	//전투로 인해 바뀐 상황들을 여기서 처리
	m_Printer->CombatModeOFF();
}

bool CGameManager::InputProcInCombat( CPlayerCharacter* m_PC_Room )
{

//	if ( IsInput ( strInput, "q") )  {
//		printf_s ("quit\n");
		//도망칠 수 있으나 패널티가 크게 작용할 수 있도록
//		return false;
//	} 
	//전투는 입력하는 형태가 아닌 gui형태로 진행예정,
	//전투시 시간이 경과함에 따라서 주변유닛들이 전투지역으로 이동할 확률 증가.


	char strInput = _getch(); 


	if ( strInput == 'w' || strInput == 'W')
		m_PC_Room->Move(DIR_UP);
	if ( strInput == 's' || strInput == 'S')
		m_PC_Room->Move ( DIR_DOWN ) ;
	if ( strInput == 'a' || strInput == 'A')
		m_PC_Room->Move ( DIR_LEFT ) ;
	if ( strInput == 'd' || strInput == 'D')
		m_PC_Room->Move ( DIR_RIGHT ) ;

	m_Printer->PrintAllThing();

	if ( m_IsCombatOccur == false )
		return false;

	return true;
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