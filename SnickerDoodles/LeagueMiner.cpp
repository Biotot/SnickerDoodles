#include "stdafx.h"
#include "LeagueMiner.h"
#include "PainterBase.h"
#include "Utility.h"

namespace SnickerDoodles
{

	LeagueMiner::LeagueMiner()
	{
	}


	LeagueMiner::~LeagueMiner()
	{
	}

	bool LeagueMiner::StartThread()
	{
		bool aRet = false;
		RunProcess();


		DWORD aPlayerList = ReadDWORD(m_Client + BasePlayerList);
		DWORD aFirstPlayer = ReadDWORD(aPlayerList + 0x0);
		SetUp();

		while (true)
		{
			Run();

			Thread::Sleep(1000);
		}
		return aRet;
	}

	bool LeagueMiner::SetUp()
	{
		bool aRet = false;
		DWORD aPlayerList = ReadDWORD(m_Client + BasePlayerList);
		DWORD aPlayer = ReadDWORD(aPlayerList + 0x0);

		float aMapSegment = MapWidth / (HeatMapNodeCount + 1);


		for (int x = 0; x < HeatMapNodeCount; x++)
		{
			for (int y = 0; y < HeatMapNodeCount; y++)
			{
				m_HeatMapNodes[x][y].m_XCoord = aMapSegment*(x + 1);
				m_HeatMapNodes[x][y].m_ZCoord = aMapSegment*(y + 1);
				m_HeatMapNodes[x][y].m_Value = 0;
			}
		}

		for (int x = 0; x < PlayerCount; x++)
		{
			DWORD aPlayer = MinerBase::ReadDWORD(aPlayerList + (0x04 * x));
			m_UnitList[x].m_BasePointer = aPlayer;
			m_UnitList[x].m_Team = ReadInt(aPlayer + PlayerTeamOffset);
			m_UnitList[x].m_ChampName = ReadString(aPlayer + PlayerChampionNameOffset);
			m_UnitList[x].m_Type = ReadInt(aPlayer + PlayerTypeOffset);
			//m_UnitList[x].m_Name = ReadString(aPlayer + PlayerNameOffset);

			m_UnitList[x].m_PlayerNumber = x;
		}

		return aRet;
	}



	/*
	Loop
	*/
	bool LeagueMiner::Run()
	{
		bool aRet = false;

		m_Clock = ReadFloat(m_Client + BaseClock);

		//printf("BLOCK \n");

		for (int x = 0; x < PlayerCount; x++)
		{

			UpdatePlayer(&m_UnitList[x]);
			if (m_UnitList[x].m_Living)
			{
				CreateMIAMap(&m_UnitList[x]);
				UpdateHeatMap(&m_UnitList[x]);
			}
		}
		CreateHeatMap();
		CalculateScores();

		m_ScreenPainter->LoadReadyArray();

		return aRet;
	}


	/*
	
	UPDATE PLAYER DATA

	*/

	bool LeagueMiner::UpdatePlayer(Unit *tUnit)
	{
		bool aRet = false;

		float aHealth = tUnit->m_Health;
		float aXCoord = tUnit->m_XCoord;
		float aZCoord = tUnit->m_ZCoord;

		tUnit->m_Health		 = ReadFloat(tUnit->m_BasePointer + PlayerHPOffset);
		tUnit->m_HealthMax	 = ReadFloat(tUnit->m_BasePointer + PlayerHPMaxOffset);
		tUnit->m_XCoord		 = ReadFloat(tUnit->m_BasePointer + PlayerCoordOffset + 0x00);
		tUnit->m_YCoord		 = ReadFloat(tUnit->m_BasePointer + PlayerCoordOffset + 0x04);
		tUnit->m_ZCoord		 = ReadFloat(tUnit->m_BasePointer + PlayerCoordOffset + 0x08);
		tUnit->m_Level		 = ReadInt(tUnit->m_BasePointer + PlayerLevelOffset);
		tUnit->m_MoveSpeed	 = ReadFloat(tUnit->m_BasePointer + PlayerMovespeedOffset);
		tUnit->m_Living		 = (642 == ReadInt(tUnit->m_BasePointer + PlayerLivingOffset));

		tUnit->m_AD			 = ReadFloat(tUnit->m_BasePointer + PlayerADOffset);
		tUnit->m_AP			 = ReadFloat(tUnit->m_BasePointer + PlayerAPOffset);
		tUnit->m_Armor		 = ReadFloat(tUnit->m_BasePointer + PlayerArmorOffset);
		tUnit->m_MagicResist = ReadFloat(tUnit->m_BasePointer + PlayerMagicResistOffset);

		if ((aHealth != tUnit->m_Health) || (aXCoord != tUnit->m_XCoord) || (aZCoord != tUnit->m_ZCoord))
		{
			tUnit->m_LastUpdated = m_Clock;
		}
		aRet = tUnit->VerifyUnit();
		return aRet;
	}


	/*
	
	CALCULATE PLAYER SCORES
	
	*/

	bool LeagueMiner::CalculateScores()
	{
		bool aRet = false;
		float aHealthMaxScoreMean = 0, aHealthMaxScoreDeviation = 0;
		float aArmorScoreMean = 0, aArmorScoreDeviation = 0;
		float aMagicResistScoreMean = 0, aMagicResistScoreDeviation = 0;
		float aADScoreMean = 0, aADScoreDeviation = 0;
		float aAPScoreMean = 0, aAPScoreDeviation = 0;

		float aHealthMaxArray[PlayerCount];
		float aArmorArray[PlayerCount];
		float aMagicResistArray[PlayerCount];
		float aADArray[PlayerCount];
		float aAPArray[PlayerCount];

		for (int x = 0; x < PlayerCount; x++)
		{
			aHealthMaxArray[x]		= m_UnitList[x].m_HealthMax;
			aArmorArray[x]			= m_UnitList[x].m_Armor;
			aMagicResistArray[x]	= m_UnitList[x].m_MagicResist;
			aADArray[x]				= m_UnitList[x].m_AD;
			aAPArray[x]				= m_UnitList[x].m_AP;

			aHealthMaxScoreMean		+= m_UnitList[x].m_HealthMax;
			aArmorScoreMean			+= m_UnitList[x].m_Armor;
			aMagicResistScoreMean	+= m_UnitList[x].m_MagicResist;
			aADScoreMean			+= m_UnitList[x].m_AD; 
			aAPScoreMean			+= m_UnitList[x].m_AP;

		}

		aHealthMaxScoreMean		= aHealthMaxScoreMean / PlayerCount;
		aArmorScoreMean			= aArmorScoreMean / PlayerCount;
		aMagicResistScoreMean	= aMagicResistScoreMean / PlayerCount;
		aADScoreMean			= aADScoreMean / PlayerCount;
		aAPScoreMean			= aAPScoreMean / PlayerCount;

		aHealthMaxScoreDeviation	= Utility::StandardDeviation(aHealthMaxArray, PlayerCount);
		aArmorScoreDeviation		= Utility::StandardDeviation(aArmorArray, PlayerCount);
		aMagicResistScoreDeviation	= Utility::StandardDeviation(aMagicResistArray, PlayerCount);
		aADScoreDeviation			= Utility::StandardDeviation(aADArray, PlayerCount);
		aAPScoreDeviation			= Utility::StandardDeviation(aAPArray, PlayerCount);

		//Add the header for the data
		DrawnImage aDataStringHeader;
		aDataStringHeader.m_DrawType = "TabData";
		aDataStringHeader.m_Type = "PlayerData";
		aDataStringHeader.m_Text = "Champ Name | Score | AD | AP | HP | AR | MR ";
		aDataStringHeader.m_X = PlayerDataStringX;
		aDataStringHeader.m_Y = PlayerDataStringY-25;
		aDataStringHeader.m_Color = 3;
		m_ScreenPainter->AddImage(aDataStringHeader);


		float aRedADScore = 0;
		float aRedAPScore = 0;
		float aBlueADScore = 0;
		float aBlueAPScore = 0;


		//Loop through players and assign the score values to the right spots
		for (int x = 0; x < PlayerCount; x++)
		{
			m_UnitList[x].m_HealthMaxScore		= (m_UnitList[x].m_HealthMax - aHealthMaxScoreMean) / aHealthMaxScoreDeviation;
			m_UnitList[x].m_ArmorScore			= (m_UnitList[x].m_Armor - aArmorScoreMean) / aArmorScoreDeviation;
			m_UnitList[x].m_MagicResistScore	= (m_UnitList[x].m_MagicResist - aMagicResistScoreMean) / aMagicResistScoreDeviation;
			m_UnitList[x].m_ADScore				= 2*(m_UnitList[x].m_AD - aADScoreMean) / aADScoreDeviation;
			m_UnitList[x].m_APScore				= 2*(m_UnitList[x].m_AP - aAPScoreMean) / aAPScoreDeviation;

			if (m_UnitList[x].m_Team == 100)
			{
				aBlueADScore += m_UnitList[x].m_ADScore;
				aBlueAPScore += m_UnitList[x].m_APScore;
			}
			else
			{
				aRedADScore += m_UnitList[x].m_ADScore;
				aRedAPScore += m_UnitList[x].m_APScore;
			}

			//PlayerScore is the combined values of all scores
			m_UnitList[x].m_PlayerScore = m_UnitList[x].m_HealthMaxScore + m_UnitList[x].m_ArmorScore
				+ m_UnitList[x].m_MagicResistScore + m_UnitList[x].m_ADScore + m_UnitList[x].m_APScore;

			CString aPlayerData = "";
			aPlayerData.AppendFormat("%s \t| %.1f \t| %.1f \t| %.1f \t| %.1f \t| %.1f \t| %.1f ", m_UnitList[x].m_ChampName, m_UnitList[x].m_PlayerScore, m_UnitList[x].m_ADScore,
				m_UnitList[x].m_APScore, m_UnitList[x].m_HealthMaxScore, m_UnitList[x].m_ArmorScore, m_UnitList[x].m_MagicResistScore);

			DrawnImage aDataString;
			aDataString.m_DrawType = "TabData";
			aDataString.m_Type = "PlayerData";
			aDataString.m_Text = aPlayerData;
			aDataString.m_X = PlayerDataStringX;
			aDataString.m_Y = PlayerDataStringY + (20*x);
			aDataString.m_Color = Utility::ColorRange(m_UnitList[x].m_PlayerScore, -2, 2);
			m_ScreenPainter->AddImage(aDataString);
		}

		CString aTeamDamageTypeString = "";
		aTeamDamageTypeString.AppendFormat("Blue: AD: %.1f AP: %.1f || Red: AD: %.1f AP: %.1f", aBlueADScore, aBlueAPScore, aRedADScore, aRedAPScore);

		DrawnImage aTeamDamage;
		aTeamDamage.m_DrawType = "TabData";
		aTeamDamage.m_Type = "PlayerData";
		aTeamDamage.m_Text = aTeamDamageTypeString;
		aTeamDamage.m_X = PlayerDataStringX;
		aTeamDamage.m_Y = PlayerDataStringY - 50;
		aTeamDamage.m_Color = 3;
		m_ScreenPainter->AddImage(aTeamDamage);

		return aRet;
	}

	/*
	
	MIA MAP AND TRACKER

	*/

	bool LeagueMiner::CreateMIAMap(Unit *tUnit)
	{
		bool aRet = false;
		float aDT = m_Clock - tUnit->m_LastUpdated;

		//If mia for longer than 5 seconds
		if (aDT > 5.0)
		{
			int aColor = 3;

			//Check how long they've been MIA. 
			if (aDT > HelpRange)
			{
				aColor = 0;
			}
			else if (aDT > (HelpRange * (2 / 3.0)))
			{
				aColor = 1;
			}
			else if (aDT > (HelpRange * (1 / 3.0)))
			{
				aColor = 2;
			}

			//Last known location box
			DrawnImage aImage;
			aImage.m_DrawType = "MiniMap";
			aImage.m_Type = "Box";
			aImage.m_X = tUnit->m_XCoord;
			aImage.m_Y = tUnit->m_ZCoord;
			aImage.m_Width = 4;
			aImage.m_Height = 4;
			aImage.m_Color = aColor;
			m_ScreenPainter->AddImage(aImage);

			//Champion Initial located above the MIA box
			DrawnImage aChampInitialImage;
			aChampInitialImage.m_DrawType = "MiniMap";
			aChampInitialImage.m_Type = "ChampInitial";
			aChampInitialImage.m_Text = tUnit->m_ChampName[0];
			aChampInitialImage.m_X = tUnit->m_XCoord;
			aChampInitialImage.m_Y = tUnit->m_ZCoord;
			aChampInitialImage.m_Color = 3;
			m_ScreenPainter->AddImage(aChampInitialImage);

		}
		return aRet;
	}

	/*
	
	HEAT MAP SHIT HERE
	
	*/
	bool LeagueMiner::UpdateHeatMap(Unit *tUnit)
	{
		bool aRet = false;
		float MoveSpeed = tUnit->m_MoveSpeed;
		if (tUnit->m_Living)
		{
			for (int x = 0; x < HeatMapNodeCount; x++)
			{
				for (int y = 0; y < HeatMapNodeCount; y++)
				{

					float aDX = tUnit->m_XCoord - m_HeatMapNodes[x][y].m_XCoord;
					float aDZ = tUnit->m_ZCoord - m_HeatMapNodes[x][y].m_ZCoord;
					aDX = aDX * aDX;
					aDZ = aDZ * aDZ;

					float aDT = m_Clock - tUnit->m_LastUpdated;
					float aTimeToTarget = sqrt(aDX + aDZ) / MoveSpeed;
					aTimeToTarget = aTimeToTarget - aDT;

					//If the player is too far then they count for less
					float aThreatMultiplier = 5;
					float aConfidenceMultiplier = ((HelpRange - aTimeToTarget) / HelpRange);
					float aDistanceMultiplier = ((ConfidenceRange - aDT) / ConfidenceRange);

					if (aTimeToTarget > 1)
					{
						aThreatMultiplier = aThreatMultiplier / (aTimeToTarget);
					}

					// aConfidenceMultiplier * aDistanceMultiplier;

					//tUnit->m_Level
					float aThreat = aThreatMultiplier * tUnit->m_Level;

					if (tUnit->m_Team == 200)
					{
						aThreat = -aThreat;
					}

					m_HeatMapNodes[x][y].m_Value += aThreat;
				}
			}
		}

		return aRet;
	}

	bool LeagueMiner::CreateHeatMap()
	{
		bool aRet = false;
		CString aOutput = "";
		CString aOutput2 = "";
		float aMean = 0;
		float aDeviation = 0;

		float NodeValues[HeatMapNodeCount*HeatMapNodeCount];


		for (int y = 0; y < HeatMapNodeCount; y++)
		{
			for (int x = 0; x < HeatMapNodeCount; x++)
			{
				NodeValues[x+(HeatMapNodeCount*y)] = m_HeatMapNodes[x][y].m_Value;
				aMean += m_HeatMapNodes[x][y].m_Value;
			}
		}
		aMean = aMean/(HeatMapNodeCount*HeatMapNodeCount);
		aDeviation = Utility::StandardDeviation(NodeValues, HeatMapNodeCount*HeatMapNodeCount);

		for (int y = HeatMapNodeCount - 1; y >= 0; y--)
		{
			for (int x = 0; x < HeatMapNodeCount; x++)
			{
				float aDev = m_HeatMapNodes[x][y].m_Value - aMean;
				if (aDeviation != 0)
				{
					m_HeatMapNodes[x][y].m_Difference = aDev / aDeviation;
				}
				else
				{
					m_HeatMapNodes[x][y].m_Difference = 0;
				}

				int aColor = -1;

				if ((m_HeatMapNodes[x][y].m_Difference) < (0.0))
				{
					aOutput.AppendFormat("%.1f", m_HeatMapNodes[x][y].m_Difference);
				}
				else
				{
					aOutput.AppendFormat(" %.1f", m_HeatMapNodes[x][y].m_Difference);
				}

				if ((m_HeatMapNodes[x][y].m_Difference) > (0.5))
				{
					if ((m_HeatMapNodes[x][y].m_Difference) > (1))
					{
						aOutput2.Append(" X");
						aColor = 3;
					}
					else
					{
						aOutput2.Append(" x");
						aColor = 2;
					}
				}
				else if ((m_HeatMapNodes[x][y].m_Difference) < (-0.5) && aDev)
				{
					if ((m_HeatMapNodes[x][y].m_Difference) < (-1))
					{
						aOutput2.Append(" O");
						aColor = 0;
					}
					else
					{
						aOutput2.Append(" o");
						aColor = 1;
					}
				}
				else
				{
					aOutput2.Append("  ");
				}

				if (aColor != -1)
				{

					DrawnImage aImage;
					aImage.m_DrawType = "MiniMap";
					aImage.m_Type = "Box";
					aImage.m_X = m_HeatMapNodes[x][y].m_XCoord;
					aImage.m_Y = m_HeatMapNodes[x][y].m_ZCoord;
					aImage.m_Width = 2;
					aImage.m_Height = 2;
					aImage.m_Color = aColor;
					m_ScreenPainter->AddImage(aImage);
				}

				m_HeatMapNodes[x][y].m_Value = 0;
			}
			//printf(aOutput + "\t|");
			//printf(aOutput2 + "|\n");
		}
		//printf("Block \n");
		return aRet;
	}

	void LeagueMiner::SetScreenPainter(PainterBase *tScreenPainter)
	{
		m_ScreenPainter = tScreenPainter;
	}



}