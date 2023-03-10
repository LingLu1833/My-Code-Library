#include "remote_memory.h"

#include <stdexcept>

// 显式实例化 需要使用什么类型的内存读写在这里添加 格式如下
/*
template int RemoteMemory::readMemory<int>(LPVOID address);
template void RemoteMemory::writeMemory<int>(LPVOID address, int value);
*/

RemoteMemory::RemoteMemory(DWORD processId)
{
	m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!m_processHandle) 
	{
		throw std::runtime_error("Failed to open process");
	}
}

RemoteMemory::~RemoteMemory()
{
	if (m_processHandle)
	{
		CloseHandle(m_processHandle);
	}
}

template<typename T>
T RemoteMemory::readMemory(LPVOID address)
{
	T buffer{};
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(m_processHandle, address, &buffer, sizeof(T), &bytesRead) || bytesRead != sizeof(T)) 
	{
		throw std::runtime_error("Failed to read memory");
	}
	return buffer;
}

template<typename T>
void RemoteMemory::writeMemory(LPVOID address, T value)
{
	SIZE_T bytesWritten = 0;
	if (!WriteProcessMemory(m_processHandle, address, &value, sizeof(T), &bytesWritten) || bytesWritten != sizeof(T))
	{
		throw std::runtime_error("Failed to write memory");
	}
}
