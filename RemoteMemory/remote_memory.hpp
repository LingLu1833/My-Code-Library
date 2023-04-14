#pragma once
#include "pch.h"
#include <TlHelp32.h>

class RemoteMemory
{
public:
	RemoteMemory(DWORD processId);
	RemoteMemory(LPCSTR processName);
	~RemoteMemory();

	DWORD GetModuleBaseAddress(LPCSTR lpModuleName);

	template <typename T>
	T readMemory(LPVOID address);

	template <typename T>
	void writeMemory(LPVOID address, T value);

private:
	DWORD GetProcessIDByName(LPCSTR processName);

private:
	DWORD m_processId;
	HANDLE m_processHandle;
};

template<typename T>
T RemoteMemory::readMemory(LPVOID address)
{
	T buffer{};
	ReadProcessMemory(m_processHandle, address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T>
void RemoteMemory::writeMemory(LPVOID address, T value)
{
	SIZE_T bytesWritten = 0;
	if (!WriteProcessMemory(m_processHandle, address, &value, sizeof(T), &bytesWritten) || bytesWritten != sizeof(T))
	{
		cout << "Failed to write memory." << endl;
	}
}
