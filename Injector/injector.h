#pragma once
#include <Windows.h>

class Injector
{
public:
	Injector(DWORD pid, LPCWSTR dllPath);
	~Injector();

	bool Inject();

	bool Unload();

private:
	DWORD m_pid;
	LPCWSTR m_dllPath;
};

