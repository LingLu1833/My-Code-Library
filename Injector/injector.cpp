#include "injector.h"

Injector::Injector(DWORD pid, LPCWSTR dllPath)
{
	m_pid = pid;
	m_dllPath = dllPath;
}

Injector::~Injector()
{
}

bool Injector::Inject()
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
	if (hProcess == NULL)
	{
		return false;
	}

	LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, wcslen(m_dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (pRemoteMem == NULL)
	{
		CloseHandle(hProcess);
		return false;
	}

	if (!WriteProcessMemory(hProcess, pRemoteMem, m_dllPath, wcslen(m_dllPath) + 1, NULL)) {
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL)
	{
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	FARPROC pLoadLibraryW = GetProcAddress(hKernel32, "LoadLibraryW");
	if (pLoadLibraryW == NULL)
	{
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, pRemoteMem, 0, NULL);
	if (hThread == NULL)
	{
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}

bool Injector::Unload()
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
	if (hProcess == NULL)
	{
		return false;
	}

	HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL)
	{
		CloseHandle(hProcess);
		return false;
	}

	FARPROC pFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");
	if (pFreeLibrary == NULL)
	{
		CloseHandle(hProcess);
		return false;
	}

	HMODULE hModule = GetModuleHandle(m_dllPath);
	if (hModule == NULL)
	{
		CloseHandle(hProcess);
		return false;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFreeLibrary, (LPVOID)hModule, 0, NULL);
	if (hThread == NULL)
	{
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}
