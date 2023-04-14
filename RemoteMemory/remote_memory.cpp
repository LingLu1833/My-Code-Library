#include "remote_memory.hpp"

RemoteMemory::RemoteMemory(DWORD processId) : m_processId(processId)
{
	m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!m_processHandle)
	{
		cout << "Failed to open process." << endl;
	}
}

RemoteMemory::RemoteMemory(LPCSTR processName)
{
	DWORD processId = GetProcessIDByName(processName);
	if (!processId)
	{
		cout << "Get process ID Failed." << endl;
	}
	m_processId = processId;

	m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!m_processHandle)
	{
		cout << "Failed to open process." << endl;
	}
}

RemoteMemory::~RemoteMemory()
{
	if (m_processHandle)
	{
		CloseHandle(m_processHandle);
	}
}

DWORD RemoteMemory::GetModuleBaseAddress(LPCSTR lpModuleName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_processId);
	MODULEENTRY32 moduleEntry32 = { 0 };
	moduleEntry32.dwSize = sizeof(moduleEntry32);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	if (Module32First(hSnapshot, &moduleEntry32))
	{
		do
		{
			if (strcmp(moduleEntry32.szModule, lpModuleName) == 0)
			{
				CloseHandle(hSnapshot);
				return (DWORD)moduleEntry32.modBaseAddr;
			}
		} while (Module32Next(hSnapshot, &moduleEntry32));
	}

	CloseHandle(hSnapshot);
	return NULL;
}

DWORD RemoteMemory::GetProcessIDByName(LPCSTR processName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(pe32);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (strcmp(pe32.szExeFile, processName) == 0) {
				CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return 0;
}
