#pragma once

#include <Windows.h>
#include <DbgEng.h>


#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif // __cplusplus


//
// Macros
//

#define HEAPALLOC(cbSize) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (cbSize))

#define HEAPFREE(pvMemory)							\
	if (NULL != (pvMemory))							\
	{												\
		HeapFree(GetProcessHeap(), 0, (pvMemory));	\
		(pvMemory) = NULL;							\
	}

#define RELEASE_INTERFACE(piInterface)							\
	if (NULL != (piInterface))									\
	{															\
		(VOID)((piInterface)->lpVtbl->Release(piInterface));	\
		(piInterface) = NULL;									\
	}


//
// Functions
//

HRESULT UTILS_OutputString(
	_In_	IDebugClient *	piClient,
	_In_	ULONG			nMask,
	_In_	PCSTR			pszFormat,
	...
	);


#ifdef __cplusplus
}
#endif // __cplusplus
