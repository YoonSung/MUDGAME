#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"
#include <atlstr.h>
#include "Room.h"

#define MAX_LOG_NUM 10

class CPrinter
{
public:
	CPrinter::CPrinter(CPlayerCharacter& PC);
	~CPrinter(void);
	void AddLogBuffer(std::string logString);
	void ShowScreen();
	void AutoMapDisplayON();
	void AutoMapDisPlayOFF();
	void CombatModeON();
	void CombatModeOFF();
	void PrintExceptEnemy();
};

