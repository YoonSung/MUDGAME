#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"
#include "Printer.h"

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
	bool InputProcInCombat();
protected:
	CGameMap* m_Map;
	CPlayerCharacter * m_PC;
	CPrinter* m_Printer;
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

