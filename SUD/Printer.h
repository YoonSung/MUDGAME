#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"
#include <atlstr.h>

#define MAX_LOG_NUM 10

class CPrinter
{
public:
	CPrinter(CPlayerCharacter& PC, CGameMap& MAP);
	~CPrinter(void);
	void AddLogBuffer(std::string logString);
	void ShowScreen();
	void AutoMapDisplayON();
	void AutoMapDisPlayOFF();
	void CombatModeON();
	void CombatModeOFF();
	void PrintExceptEnemy();
};

