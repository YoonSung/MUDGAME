#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"

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
private:
	bool IsInput( std::string inputValue, const char* compareValue);
	void CreateMobs();
	void AutoMapDisplay();
	void AutoMapDisplayON();
	void AutoMapDisPlayOFF();
	void CheckCombatOccur();
	void OccurCombat();
	void EndCombat();
	bool CheckMissionClear();
	void EndMission();
	void destroyMonster(int x, int y);
};

