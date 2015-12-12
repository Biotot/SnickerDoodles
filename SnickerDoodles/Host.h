#pragma once
#include "LeagueMiner.h"
#include "PainterBase.h"
using namespace System;
using namespace System::Threading;

namespace SnickerDoodles
{

	class Host
	{
	public:
		Host(); 
		~Host();

		void StartMiner();
		void StartPainter();



	private:

		MinerBase *m_DataMiner;
		PainterBase *m_ScreenPainter;
	};

}