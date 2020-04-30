#pragma once

#include "defines.h"

#include <Pdh.h>
#include <PdhMsg.h>

#pragma comment(lib, "pdh.lib")


BOOL PdhGetInstances(
	LPCWSTR objectName,
	LPWSTR * instanceListBuffer,
	PDWORD instanceListSize,
	LPWSTR * counterListBuffer,
	PDWORD counterListSize,
	DWORD detailLevel
);

BOOL PdhGetCounter(
	PDH_COUNTER_PATH_ELEMENTS_W counterPathElements,
	DWORD dwFormat,
	PPDH_FMT_COUNTERVALUE fmtValue
);

BOOL FillCounterPathStructure(
	LPWSTR objectName,
	LPWSTR counterName,
	LPWSTR instanceName,
	PPDH_COUNTER_PATH_ELEMENTS_W counterPathElements
);
