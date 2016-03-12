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
// Typedefs
//

typedef struct _HOOK_DESCRIPTOR
{
	FARPROC	pfnFunctionToHook;
	FARPROC	pfnHookFunction;
} HOOK_DESCRIPTOR, *PHOOK_DESCRIPTOR;


//
// Functions
//

HRESULT REGFIXHELPER_Initialize(
	_In_								IDebugClient *		piClient,
	_Outptr_result_buffer_(*pnHooks)	PHOOK_DESCRIPTOR *	pptDescriptors,
	_Out_								PDWORD				pnHooks
	);


#ifdef __cplusplus
}
#endif // __cplusplus

