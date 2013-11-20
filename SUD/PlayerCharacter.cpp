#include "stdafx.h"
#include "PlayerCharacter.h"


CPlayerCharacter::CPlayerCharacter(void)
{
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
