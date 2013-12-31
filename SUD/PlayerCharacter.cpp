#include "stdafx.h"
#include "PlayerCharacter.h"


CPlayerCharacter::CPlayerCharacter(void)
{
	skill_1_num = 1;
	skill_2_num = 0;
	skill_3_num = 0;
	m_Total_Killing_num = 0;
}


CPlayerCharacter::~CPlayerCharacter(void)
{
}

int CPlayerCharacter::GetPositionX()
{
	return m_position.x;
}

int CPlayerCharacter::GetPositionY()
{
	return m_position.y;
}

void CPlayerCharacter::LevelUpgrade()
{
	m_level++;
	RefreshSkillNumber();
}

void CPlayerCharacter::RefreshSkillNumber()
{
	if ( m_level <= 5)
	{
		skill_1_num = 1;
	}
	else
	{
		skill_1_num = (int)(m_level/3);
	}


	if ( 2 <= m_level && m_level <= 5 )
	{
		skill_2_num = 2;
	} 
	else if ( m_level > 5 )
	{
		skill_2_num = (int)(m_level/5);
	}


	if ( 5<= m_level && m_level <= 7)
	{
		skill_3_num = 1;
	}
	else if ( m_level > 8 )
	{
		skill_3_num = (int)(m_level/10);
	}
}

void CPlayerCharacter::CheckUpdate()
{
	if ( m_experience >= 100 )
	{
		LevelUpgrade();
		m_experience = 0;
	}
}

void CPlayerCharacter::KillingZombie( int experience )
{
	m_experience += experience;
	m_Total_Killing_num++;
}

void CPlayerCharacter::AttackFromEnemy( int damage )
{
	m_energy -= damage;
}

bool CPlayerCharacter::IsPlayerDead()
{
	return ( 0 >= m_energy );
}

int CPlayerCharacter::GetTotalKillingNum()
{
	return m_Total_Killing_num;
}

void CPlayerCharacter::SetTotalKillingNum( int killingNum )
{
	m_Total_Killing_num = killingNum;
}
