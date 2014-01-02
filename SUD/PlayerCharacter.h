#pragma once
#include "character.h"
#include "Monster.h"

class CPlayerCharacter :
public CCharacter
{
public:
	CPlayerCharacter(void);
	virtual ~CPlayerCharacter(void);
	void CheckUpdate();
	void LevelUpgrade();
	void KillingZombie(int experience);
	void AttackFromEnemy(int damage);
	int GetPositionX();
	int GetPositionY();
	void RefreshSkillNumber();
	bool IsPlayerDead();
	int GetTotalKillingNum();
	void SetTotalKillingNum(int killingNum);
private:
	int m_Total_Killing_num;
	int skill_1_num;
	int skill_2_num;
	int skill_3_num;
};

