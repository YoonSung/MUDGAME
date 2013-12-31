#pragma once
#include "character.h"

class CPlayerCharacter :
public CCharacter
{
public:
	CPlayerCharacter(void);
	~CPlayerCharacter(void);
	void CheckUpdate();
	void LevelUpgrade();
	int GetPositionX();
	int GetPositionY();
	void RefreshSkillNumber();
private:
	int skill_1_num;
	int skill_2_num;
	int skill_3_num;
};

