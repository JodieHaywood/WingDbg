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

#define HEAPFREE(pvMemory)								\
	do													\
	{													\
		if (NULL != (pvMemory))							\
		{												\
			HeapFree(GetProcessHeap(), 0, (pvMemory));	\
			(pvMemory) = NULL;							\
		}												\
	} while (0)

#define RELEASE_INTERFACE(piInterface)								\
	do																\
	{																\
		if (NULL != (piInterface))									\
		{															\
			(VOID)((piInterface)->lpVtbl->Release(piInterface));	\
			(piInterface) = NULL;									\
		}															\
	} while (0)


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
