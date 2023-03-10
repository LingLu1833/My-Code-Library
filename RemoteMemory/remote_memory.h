#pragma once
#include <Windows.h>

class RemoteMemory
{
public:
	RemoteMemory(DWORD processId);
	~RemoteMemory();

	template <typename T>
	T readMemory(LPVOID address);

	template <typename T>
	void writeMemory(LPVOID address, T value);

private:
	HANDLE m_processHandle;
};
