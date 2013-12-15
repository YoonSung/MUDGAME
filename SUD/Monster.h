#pragma once
#include "character.h"

class CMonster :
	public CCharacter
{
public:
	CMonster( int startX, int startY );
	~CMonster(void);
	virtual void Move ( DIRECTION dir );
private:
	CMonster(void);
	virtual void SetPosition ( Position pos );
	virtual void SetPosition (int x, int y);
};

