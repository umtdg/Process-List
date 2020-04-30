#pragma once

#include "../includes/PdhHelpers.h"

BOOL PdhGetInstances(
	LPCWSTR objectName,
	LPWSTR * instanceListBuffer,
	PDWORD instanceListSize,
	LPWSTR * counterListBuffer,
	PDWORD counterListSize,
	DWORD detailLevel
)
{
	ReturnIfNull(objectName, FALSE);
	ReturnIfNull(instanceListBuffer, FALSE);
	ReturnIfNull(instanceListSize, FALSE);

	detailLevel = (detailLevel == 0) ? PERF_DETAIL_WIZARD : detailLevel;

	PDH_STATUS status;
	LPWSTR lpInstanceListBuffer = NULL;
	DWORD dwInstanceListSize = 0;
	LPWSTR lpCounterListBuffer = NULL;
	DWORD dwCounterListSize = 0;

	status = PdhEnumObjectItemsW(
		NULL,
		NULL,
		objectName,
		lpCounterListBuffer,
		&dwCounterListSize,
		lpInstanceListBuffer,
		&dwInstanceListSize,
		detailLevel,
		0
	);
	if (status == PDH_MORE_DATA)
	{
		SafeAlloc(
			lpInstanceListBuffer,
			dwInstanceListSize * sizeof(WCHAR),
			WCHAR
		);

		SafeAlloc(
			lpCounterListBuffer,
			dwCounterListSize * sizeof(WCHAR),
			WCHAR
		);

		status = PdhEnumObjectItemsW(
			NULL,
			NULL,
			objectName,
			lpCounterListBuffer,
			&dwCounterListSize,
			lpInstanceListBuffer,
			&dwInstanceListSize,
			detailLevel,
			0
		);
		PdhError(status, FALSE);
	}

	*instanceListBuffer = lpInstanceListBuffer;
	*instanceListSize = dwInstanceListSize;

	if (counterListBuffer != NULL) *counterListBuffer = lpCounterListBuffer;
	else SafeFree(lpCounterListBuffer, dwCounterListSize * sizeof(WCHAR));

	if (counterListSize != NULL) *counterListSize = dwCounterListSize;

	return TRUE;
}

BOOL FillCounterPathStructure(
	LPWSTR objectName,
	LPWSTR counterName,
	LPWSTR instanceName,
	PPDH_COUNTER_PATH_ELEMENTS_W counterPathElements
)
{
	ReturnIfNull(counterPathElements, FALSE);

	counterPathElements->szMachineName = NULL;
	counterPathElements->szObjectName = objectName;
	counterPathElements->szInstanceName = instanceName;
	counterPathElements->szParentInstance = NULL;
	counterPathElements->dwInstanceIndex = 0;
	counterPathElements->szCounterName = counterName;

	return TRUE;
}

BOOL PdhGetCounter(
	PDH_COUNTER_PATH_ELEMENTS_W counterPathElements,
	DWORD dwFormat,
	PPDH_FMT_COUNTERVALUE fmtValue
)
{
	ReturnIfNull(fmtValue, FALSE);

	PDH_STATUS status;
	HQUERY hQuery;
	HCOUNTER hCounter;
	WCHAR szCounterPath[PDH_MAX_COUNTER_PATH];
	DWORD dwPathLength;

	status = PdhOpenQueryW(
		NULL,
		0,
		&hQuery
	);
	PdhError(status, FALSE);

	dwPathLength = PDH_MAX_COUNTER_PATH;
	status = PdhMakeCounterPathW(
		&counterPathElements,
		szCounterPath,
		&dwPathLength,
		0
	);
	PdhError(status, FALSE);

	status = PdhAddCounterW(
		hQuery,
		szCounterPath,
		0,
		&hCounter
	);
	PdhError(status, FALSE);

	status = PdhCollectQueryData(hQuery);
	PdhError(status, FALSE);

	status = PdhGetFormattedCounterValue(
		hCounter,
		dwFormat,
		(LPDWORD)NULL,
		fmtValue
	);
	PdhError(status, FALSE);

	status = PdhCloseQuery(hQuery);
	PdhError(status, FALSE);

	return TRUE;
}
