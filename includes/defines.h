#pragma once

#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


#define ReturnIfNull(x, code) if ((x) == NULL) { return (code); }

#define ReturnIfFalse(x, code) if (!(x)) { return (code); }

#define ReturnIfTrue(x, code) if ((x)) { return (code); }

#define ReturnIfFail(x, code) if ((x) != ERROR_SUCCESS) { return (code); }


#define PdhErrorMessage(status, message, code) if ((status) != ERROR_SUCCESS) { wprintf_s(TEXT(message)); return (code); }

#define PdhError(status, code) ReturnIfFail(status, code)


#define Win32ErrorMessage(result, message, code) if (!(result)) { wprintf_s(TEXT(message)); return (code); }

#define Win32Error(result, code) ReturnIfFalse(result, code)


#define SafeAlloc(buf, size, type) buf = (type *)malloc((size)); if ((buf) == NULL) { exit(0); }

#define SafeFree(buf, size) if ((buf) != NULL) { ZeroMemory(buf, size); free(buf); buf = NULL; }


#define ARRAY_SIZE_BASE (128)

#define MAX_PROC_NAME_SIZE (2048)
