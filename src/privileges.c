#pragma once

#include "../includes/privileges.h"

BOOL SetPrivilege(HANDLE hToken, LPCWSTR lpName, BOOL bEnable)
{
	ReturnIfNull(hToken, FALSE);
	ReturnIfNull(lpName, FALSE);
	ReturnIfTrue(wcslen(lpName) <= 0, FALSE);

	BOOL result;
	LUID luid;
	TOKEN_PRIVILEGES priv;

	ZeroMemory(&luid, sizeof(LUID));
	result = LookupPrivilegeValueW(
		NULL,
		lpName,
		&luid
	);
	Win32Error(result, FALSE);

	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = luid;
	priv.Privileges[0].Attributes = (bEnable) ? SE_PRIVILEGE_ENABLED : 0;
	result = AdjustTokenPrivileges(
		hToken,
		FALSE,
		&priv,
		sizeof(TOKEN_PRIVILEGES),
		NULL,
		NULL
	);
	Win32Error(result, FALSE);

	return TRUE;
}

BOOL EnablePrivilege(HANDLE hToken, LPCWSTR lpName)
{
	return SetPrivilege(hToken, lpName, TRUE);
}

BOOL DisablePrivilege(HANDLE hToken, LPCWSTR lpName)
{
	return SetPrivilege(hToken, lpName, FALSE);
}
