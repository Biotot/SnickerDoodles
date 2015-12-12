#pragma once
class Unit
{
public:
	Unit();
	~Unit();

	/*
	
	Basic Player Data
	
	*/
	DWORD m_BasePointer;
	CString m_ChampName;
	int m_Team;
	int m_Type;
	int m_Level;
	int m_PlayerNumber;
	float m_LastUpdated;
	float m_MoveSpeed;
	bool m_Living;

	float m_XCoord;
	float m_YCoord;
	float m_ZCoord;

	/*
	
	Player Values
	
	*/


	CString m_Name;
	float m_Health;
	float m_HealthMax;
	float m_Mana;
	float m_ManaMax;
	float m_Armor;
	float m_MagicResist;
	float m_AD;
	float m_AP;

	float m_HealthMaxScore;
	float m_ArmorScore;
	float m_MagicResistScore;
	float m_ADScore;
	float m_APScore;

	float m_PlayerScore;

	bool VerifyUnit();

};

