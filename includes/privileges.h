#pragma once

#include "defines.h"

BOOL SetPrivilege(HANDLE hToken, LPCWSTR lpName, BOOL bEnable);

BOOL EnablePrivilege(HANDLE hToken, LPCWSTR lpName);

BOOL DisablePrivilege(HANDLE hToken, LPCWSTR lpName);
