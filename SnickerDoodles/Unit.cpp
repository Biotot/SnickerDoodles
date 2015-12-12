#include "stdafx.h"
#include "Unit.h"


Unit::Unit()
{
}


Unit::~Unit()
{
}


bool Unit::VerifyUnit()
{
	bool aRet = true;

	if (m_Team != 100 && m_Team != 200 && m_Team != 300)
	{
		printf("Team not correct %i\n", m_Team);
		aRet = false;
	}
	if (m_XCoord < 0 || m_XCoord > 15000)
	{
		printf("XCoord Out of bounds %f\n", m_XCoord);
		aRet = false;
	}
	if (m_ZCoord < 0 || m_ZCoord > 15000)
	{
		printf("ZCoord Out of bounds %f\n", m_ZCoord);
		aRet = false;
	}
	if (m_Level < 0 || m_Level > 18)
	{
		printf("Level Out of bounds %f\n", m_Level);
		aRet = false;
	}

	if (m_MoveSpeed < 0 || m_MoveSpeed > 1000)
	{
		printf("MoveSpeed Out of bounds %f\n", m_Level);
		aRet = false;
	}

	return aRet;
}