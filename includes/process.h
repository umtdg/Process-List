#pragma once

#include "defines.h"
#include "privileges.h"
#include "PdhHelpers.h"

typedef struct
{
	LONG pid;
	WCHAR name[MAX_PROC_NAME_SIZE];
} PROCESS, *PPROCESS;




BOOL EnterDebugMode();

BOOL ExitDebugMode();

BOOL GetProcessList(PPROCESS * pProcessList, PDWORD dwProcessCount);
