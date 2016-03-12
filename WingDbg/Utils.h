#pragma once

#include <Windows.h>
#include <DbgEng.h>


#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif // __cplusplus


HRESULT OutputString(
	_In_	IDebugClient *	piClient,
	_In_	ULONG			nMask,
	_In_	PCSTR			pszFormat,
	...
	);


#ifdef __cplusplus
}
#endif // __cplusplus
