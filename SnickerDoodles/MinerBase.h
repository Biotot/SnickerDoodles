#pragma once
//#include <thread>
using namespace System::Threading;
#include "MemoryNode.h"
#include "Unit.h"



namespace SnickerDoodles
{

	class MinerBase
	{
	public:
		MinerBase();
		~MinerBase();

		void RunProcess();
		bool checkIfRunning();
		DWORD GetModuleNamePointer(LPSTR LPSTRModuleName, DWORD __DwordProcessId);
		DWORD getThreadByProcess(DWORD __DwordProcess);
		DWORD FindProcessName(const char *__ProcessName, PROCESSENTRY32 *pEntry); 
		void runSetDebugPrivs();

	protected:

		CString ReadString(DWORD tOffset);
		int ReadInt(DWORD tOffset);
		float ReadFloat(DWORD tOffset);
		DWORD ReadDWORD(DWORD tOffset);
		DWORD m_Client;
		HWND m_HWND;
	private:
		
		HANDLE m_ProcessHandle;
		DWORD m_ProcessBase;



		PROCESSENTRY32 m_GameProcess;
		HANDLE __HandleProcess;

	};

}