#include "stdafx.h"
#include "MinerBase.h"


namespace SnickerDoodles
{


	MinerBase::MinerBase()
	{
	}


	MinerBase::~MinerBase()
	{
	}


	/*
	READING
	*/
	CString MinerBase::ReadString(DWORD tOffset)
	{
		CString aRet = "-1";
		char aCharList[30];
		ReadProcessMemory(__HandleProcess, (PBYTE*)(tOffset), &aCharList, 20, 0);
		aRet = aCharList;
		return aRet;
	}
	int MinerBase::ReadInt(DWORD tOffset)
	{
		int aRet = -1;
		ReadProcessMemory(__HandleProcess, (PBYTE*)(tOffset), &aRet, sizeof(int), 0);
		return aRet;
	}
	float MinerBase::ReadFloat(DWORD tOffset)
	{
		float aRet = -1;
		ReadProcessMemory(__HandleProcess, (PBYTE*)(tOffset), &aRet, sizeof(float), 0);
		return aRet;
	}
	DWORD MinerBase::ReadDWORD(DWORD tOffset)
	{
		DWORD aRet = -1;
		ReadProcessMemory(__HandleProcess, (PBYTE*)(tOffset), &aRet, sizeof(DWORD), 0);
		return aRet;
	}


	/*
	Hijacking Process and permissions
	Not refactored to standards
	*/

	void MinerBase::RunProcess()
	{
		//commented lines are for non steam versions of the game
		runSetDebugPrivs();
		LPSTR aLeagueGame = _T("League of Legends.exe");

		while (!FindProcessName(aLeagueGame, &m_GameProcess)) {
			Sleep(1000);
			cout << "Cant find game" << endl;
		}
		//while (!(getThreadByProcess(__gameProcess.th32ProcessID))) Sleep(12);
		cout << m_GameProcess.th32ProcessID << endl;
		m_Client = 0x0;
		__HandleProcess = OpenProcess(PROCESS_ALL_ACCESS, false, m_GameProcess.th32ProcessID);
		while (m_Client == 0x0) {
			m_Client = GetModuleNamePointer(aLeagueGame, m_GameProcess.th32ProcessID);
			cout << "client == 0x0 \t";
			Sleep(1000);
		}

		cout << m_Client << endl;

	}

	DWORD MinerBase::FindProcessName(const char *__ProcessName, PROCESSENTRY32 *pEntry)
	{
		PROCESSENTRY32 __ProcessEntry;
		__ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) return 0;        if (!Process32First(hSnapshot, &__ProcessEntry))
		{
			CloseHandle(hSnapshot);
			return 0;
		}
		do {
			if (!_strcmpi(__ProcessEntry.szExeFile, __ProcessName))
			{
				memcpy((void *)pEntry, (void *)&__ProcessEntry, sizeof(PROCESSENTRY32));
				CloseHandle(hSnapshot);
				return __ProcessEntry.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &__ProcessEntry));
		CloseHandle(hSnapshot);
		return 0;
	}


	DWORD MinerBase::getThreadByProcess(DWORD __DwordProcess)
	{
		THREADENTRY32 __ThreadEntry;
		__ThreadEntry.dwSize = sizeof(THREADENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

		if (!Thread32First(hSnapshot, &__ThreadEntry)) { CloseHandle(hSnapshot); return 0; }

		do {
			if (__ThreadEntry.th32OwnerProcessID == __DwordProcess)
			{
				CloseHandle(hSnapshot);
				return __ThreadEntry.th32ThreadID;
			}
		} while (Thread32Next(hSnapshot, &__ThreadEntry));
		CloseHandle(hSnapshot);
		return 0;
	}

	DWORD MinerBase::GetModuleNamePointer(LPSTR LPSTRModuleName, DWORD __DwordProcessId)
	{
		MODULEENTRY32 lpModuleEntry = { 0 };
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
		if (!hSnapShot)
			return NULL;
		lpModuleEntry.dwSize = sizeof(lpModuleEntry);
		BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
		while (__RunModule)
		{
			if (!strcmp(lpModuleEntry.szModule, LPSTRModuleName))
			{
				CloseHandle(hSnapShot);
				return (DWORD)lpModuleEntry.modBaseAddr;
			}
			__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
		}
		CloseHandle(hSnapShot);
		return NULL;
	}


	void MinerBase::runSetDebugPrivs()
	{
		HANDLE __HandleProcess = GetCurrentProcess(), __HandleToken;
		TOKEN_PRIVILEGES priv;
		LUID __LUID;
		OpenProcessToken(__HandleProcess, TOKEN_ADJUST_PRIVILEGES, &__HandleToken);
		LookupPrivilegeValue(0, "seDebugPrivilege", &__LUID);
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Luid = __LUID;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(__HandleToken, false, &priv, 0, 0, 0);
		CloseHandle(__HandleToken);
		CloseHandle(__HandleProcess);
	}

	bool MinerBase::checkIfRunning() {
		return FindProcessName("League of Legends.exe", &m_GameProcess);
	}

	/*
	
	*/
}