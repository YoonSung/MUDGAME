#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"
#include "Printer.h"
#include "Room.h"
#define DIR_ARRAY_MAXNUM 20

class CGameManager
{
public:
	CGameManager(void);
	~CGameManager(void);

	void Init();
	void Run();
	void Release();
protected:
	bool InputProc();
	bool CGameManager::InputProcInCombat( CPlayerCharacter* m_PC_Room );
protected:
	CGameMap* m_Map;
	CPlayerCharacter * m_PC;
	CPrinter* m_Printer;
	CRoom* m_Room;
private:
	bool IsInput( std::string inputValue, const char* compareValue);
	void CreateMobs();
	void CheckCombatOccur();
	void OccurCombat();
	void EndCombat();
	bool CheckMissionClear();
	void EndMission();
	void destroyMonster(int x, int y);
	void AutoAIMovementON();
	void AutoAIMovementOFF();
};

