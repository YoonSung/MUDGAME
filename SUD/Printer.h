#pragma once

#include "GameMap.h"
#include "PlayerCharacter.h"
#include <atlstr.h>
#include "Room.h"

#define MAX_LOG_NUM 10

class CPrinter
{
public:
	static CPrinter* getInstancePtr();
	static void releaseInstance();

	~CPrinter(void);
	void AddLogBuffer(std::string logString);
	void AutoMapDisplayON();
	void AutoMapDisPlayOFF();
	void CombatModeON();
	void CombatModeOFF();
	void PrintExceptEnemy();
	void PrintAllThing();
	void init(CPlayerCharacter& PC);
private:
	static CPrinter* instance;
	CPrinter::CPrinter();
};

