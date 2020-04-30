#include "../includes/process.h"

int main(void)
{
	PPROCESS processList = NULL;
	DWORD processCount = 0;

	if (!GetProcessList(&processList, &processCount))
	{
		wprintf_s(L"Error GetProcessList\n");
		return 0;
	}

	for (DWORD i = 0; i < processCount; i++)
		wprintf_s(L"%d\t%s\n", processList[i].pid, processList[i].name);

	SafeFree(processList, sizeof(PROCESS) * processCount);

	return 0;
}
