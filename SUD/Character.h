#pragma once

struct Position
{
	int x;
	int y;
};

enum DIRECTION
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

class CCharacter
{
public:
	CCharacter ( void );
	virtual ~CCharacter ( void );

	virtual void SetPosition ( Position pos );
	virtual void SetPosition (int x, int y);
	Position GetPosition(){return m_position;};
	virtual void Move ( DIRECTION dir );
protected:
	Position m_position;
	int m_damage;
	int m_energy;
	int m_level;
};

