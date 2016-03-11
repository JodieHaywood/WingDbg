#pragma once

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif // __cplusplus


typedef struct _HOOK_DESCRIPTOR
{
	FARPROC	pfnFunctionToHook;
	FARPROC	pfnHookFunction;
} HOOK_DESCRIPTOR, *PHOOK_DESCRIPTOR;


HRESULT REGFIXHELPER_Prepare(
	_In_								IDebugClient *		piClient,
	_Outptr_result_buffer_(*pnHooks)	PHOOK_DESCRIPTOR *	pptDescriptors,
	_Out_								PDWORD				pnHooks
	);


#ifdef __cplusplus
}
#endif // __cplusplus

