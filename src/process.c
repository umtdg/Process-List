#pragma once

#include "../includes/process.h"

BOOL GetProcessList(PPROCESS * pProcesses, PDWORD dwNumberOfProcesses)
{
	ReturnIfNull(pProcesses, FALSE);
	ReturnIfNull(dwNumberOfProcesses, FALSE);

	BOOL result;
	LPWSTR instanceList;
	DWORD dwInstanceListSize;
	LPWSTR temp;
	PDH_COUNTER_PATH_ELEMENTS_W counterPathElements;
	PDH_FMT_COUNTERVALUE fmtValue;
	PPROCESS processes;
	PROCESS process;
	DWORD numberOfProcesses;

	result = EnterDebugMode();
	Win32Error(result, FALSE);

	instanceList = NULL;
	dwInstanceListSize = 0;
	result = PdhGetInstances(
		L"Process",
		&instanceList,
		&dwInstanceListSize,
		NULL,
		NULL,
		0
	);
	Win32Error(result, FALSE);

	ZeroMemory(&fmtValue, sizeof(PDH_FMT_COUNTERVALUE));
	ZeroMemory(&process, sizeof(PROCESS));

	SafeAlloc(processes, sizeof(PROCESS)*ARRAY_SIZE_BASE, PROCESS);
	numberOfProcesses = 0;

	for (temp = instanceList; *temp != 0; temp += wcslen(temp) + 1)
	{
		result = FillCounterPathStructure(
			L"Process",
			L"ID Process",
			temp,
			&counterPathElements
		);
		Win32Error(result, 0);

		result = PdhGetCounter(
			counterPathElements,
			PDH_FMT_LONG,
			&fmtValue
		);
		Win32Error(result, 0);

		process.pid = fmtValue.longValue;
		wcsncpy_s(
			process.name,
			MAX_PROC_NAME_SIZE,
			temp,
			wcslen(temp) + 1
		);

		if (numberOfProcesses != 0 && numberOfProcesses % ARRAY_SIZE_BASE == 0)
		{
			PPROCESS newBlock = (PPROCESS)realloc(
				processes,
				(((size_t)ARRAY_SIZE_BASE) + ((size_t)numberOfProcesses)) * sizeof(PROCESS)
			);
			ReturnIfNull(newBlock, FALSE);

			processes = newBlock;
		}

		processes[numberOfProcesses] = process;
		numberOfProcesses++;
	}

	SafeFree(instanceList, dwInstanceListSize * sizeof(WCHAR));

	*pProcesses = processes;
	*dwNumberOfProcesses = numberOfProcesses;

	result = ExitDebugMode();
	Win32Error(result, FALSE);

	return TRUE;
}

BOOL EnterDebugMode()
{
	BOOL result;
	HANDLE hToken;

	hToken = NULL;
	result = OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		&hToken
	);
	Win32Error(result, FALSE);

	result = EnablePrivilege(hToken, SE_DEBUG_NAME);
	Win32Error(result, FALSE);

	result = CloseHandle(hToken);
	Win32Error(result, FALSE);

	return TRUE;
}

BOOL ExitDebugMode()
{
	BOOL result;
	HANDLE hToken;

	hToken = NULL;
	result = OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		&hToken
	);
	Win32Error(result, FALSE);

	result = DisablePrivilege(hToken, SE_DEBUG_NAME);
	Win32Error(result, FALSE);

	result = CloseHandle(hToken);
	Win32Error(result, FALSE);

	return TRUE;
}
