#include "stdafx.h"
#include "Host.h"

namespace SnickerDoodles
{

	public ref class ThreadManager
	{
	public:


		static LeagueMiner *m_DataMiner;
		static PainterBase *m_ScreenPainter;

		static void RunMiner()
		{
			m_DataMiner = new LeagueMiner();
			m_DataMiner->SetScreenPainter(m_ScreenPainter);
			m_DataMiner->StartThread();
			//return *aDataMiner;
		}

		static void RunPainter()
		{
			m_ScreenPainter = new PainterBase();
			m_ScreenPainter->StartThread();
			//return *aScreenPainter;
		}
		static void DoWork()
		{
			for (int i = 0; i < 3; i++)
			{
				Console::WriteLine("Working thread...");
				Thread::Sleep(100);
			}
		}
	};



	Host::Host()
	{
	}


	Host::~Host()
	{
	}

	void Host::StartMiner()
	{
		ThreadStart^ myThreadDelegate = gcnew ThreadStart(&ThreadManager::RunMiner);
		Thread^ thread = gcnew Thread(myThreadDelegate);
		thread->Start();


	}
	void Host::StartPainter()
	{
		ThreadStart^ myThreadDelegate = gcnew ThreadStart(&ThreadManager::RunPainter);
		Thread^ thread = gcnew Thread(myThreadDelegate);
		thread->Start();
	}


}