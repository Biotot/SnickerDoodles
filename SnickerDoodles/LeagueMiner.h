#pragma once
#include "MinerBase.h"
#include "LeagueMiner.h"
#include "PainterBase.h"

class PainterBase;
namespace SnickerDoodles
{
	//BasePointers
#define BasePlayerList		0x021BE208
#define BaseClock			0x01192118
#define BaseCamera			0x011C2C98

	//PlayerClass

#define	PlayerTeamOffset	0x0014
#define	PlayerTypeOffset	0x0018
#define	PlayerNameOffset	0x0020

#define	PlayerCoordOffset	0x005C
#define PlayerLivingOffset	0x00b0


#define	PlayerHPOffset		0x0154
#define	PlayerHPMaxOffset	0x0164

#define	PlayerChampionNameOffset	0x08d0

#define	PlayerADOffset			0x0a8c
#define	PlayerAPOffset			0x0a94
#define	PlayerArmorOffset		0x0b20
#define	PlayerMagicResistOffset	0x0b28
#define PlayerMovespeedOffset	0x0b44

#define PlayerLevelOffset	0x3614

#define HeatMapNodeCount	7
#define MapHeight			15000
#define MapWidth			15000
#define HelpRange			30
#define ConfidenceRange		45

#define PlayerCount			10


#define PlayerDataStringX	1000
#define PlayerDataStringY	100

	struct HeatMapNode
	{
		float m_Value;
		float m_XCoord;
		float m_ZCoord;
		float m_Difference;
	};

	class LeagueMiner :
		public MinerBase
	{
	public:

		PainterBase* m_ScreenPainter;

		bool Run();
		bool SetUp();

		bool UpdatePlayer(Unit *tUnit);

		bool CalculateScores();

		bool UpdateHeatMap(Unit *tUnit);
		bool CreateMIAMap(Unit *tUnit);
		bool CreateHeatMap();

		Unit m_UnitList[10];
		float m_Clock;
		bool StartThread();

		LeagueMiner();
		~LeagueMiner();

		HeatMapNode m_HeatMapNodes[HeatMapNodeCount][HeatMapNodeCount];

		void SetScreenPainter(PainterBase *tScreenPainter);
	};
}

