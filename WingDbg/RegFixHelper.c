#define INITGUID
#include <Windows.h>
#include <DbgEng.h>

#include <assert.h>

#include "RegFixHelper.h"


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
// Enums
//

typedef enum _HOOK_INDEX
{
	HOOK_INDEX_GET_VALUE = 0,
	HOOK_INDEX_GET_VALUES,
	HOOK_INDEX_SET_VALUE,
	HOOK_INDEX_SET_VALUES,
	HOOK_INDEX_GET_VALUES2,
	HOOK_INDEX_SET_VALUES2,

	// Must be last:
	HOOKS_COUNT
} HOOK_INDEX, *PHOOK_INDEX;


//
// Globals
//

static FARPROC g_apfnOriginalFunctions[HOOKS_COUNT] = { NULL };


//
// Functions
//

typedef HRESULT(__stdcall * PFN_GETVALUE)(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_Out_	PDEBUG_VALUE		ptValue
	);
static HRESULT __stdcall regfixhelper_GetValueHook(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_Out_	PDEBUG_VALUE		ptValue
	)
{
	HRESULT			hrResult	= E_FAIL;
	PFN_GETVALUE	pfnGetValue	= NULL;

	pfnGetValue = (PFN_GETVALUE)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUE]);

	hrResult = pfnGetValue(piThis, nRegister, ptValue);

	// Keep last status

//lblCleanup:
	return hrResult;
}

typedef HRESULT(__stdcall * PFN_GETVALUES)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);
static HRESULT __stdcall regfixhelper_GetValuesHook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	)
{
	HRESULT			hrResult		= E_FAIL;
	PFN_GETVALUES	pfnGetValues	= NULL;

	pfnGetValues = (PFN_GETVALUES)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES]);

	hrResult = pfnGetValues(piThis, nCount, pnIndices, nStartIndex, ptValues);

	// Keep last status

//lblCleanup:
	return hrResult;
}

typedef HRESULT(__stdcall * PFN_SETVALUE)(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_In_	PDEBUG_VALUE		ptValue
	);
static HRESULT __stdcall regfixhelper_SetValueHook(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_In_	PDEBUG_VALUE		ptValue
	)
{
	HRESULT			hrResult	= E_FAIL;
	PFN_SETVALUE	pfnSetValue	= NULL;

	pfnSetValue = (PFN_SETVALUE)(g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUE]);

	hrResult = pfnSetValue(piThis, nRegister, ptValue);

	// Keep last status

//lblCleanup:
	return hrResult;
}

typedef HRESULT(__stdcall * PFN_SETVALUES)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);
static HRESULT __stdcall regfixhelper_SetValuesHook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	)
{
	HRESULT			hrResult		= E_FAIL;
	PFN_SETVALUES	pfnSetValues	= NULL;

	pfnSetValues = (PFN_SETVALUES)(g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUES]);

	hrResult = pfnSetValues(piThis, nCount, pnIndices, nStartIndex, ptValues);

	// Keep last status

//lblCleanup:
	return hrResult;
}

typedef HRESULT(__stdcall * PFN_GETVALUES2)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);
static HRESULT __stdcall regfixhelper_GetValues2Hook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	)
{
	HRESULT			hrResult		= E_FAIL;
	PFN_GETVALUES2	pfnGetValues2	= NULL;

	pfnGetValues2 = (PFN_GETVALUES2)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES2]);

	hrResult = pfnGetValues2(piThis, eSource, nCount, pnIndices, nStartIndex, ptValues);

	// Keep last status

//lblCleanup:
	return hrResult;
}

typedef HRESULT(__stdcall * PFN_SETVALUES2)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);
static HRESULT __stdcall regfixhelper_SetValues2Hook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	)
{
	HRESULT			hrResult		= E_FAIL;
	PFN_SETVALUES2	pfnSetValues2	= NULL;

	pfnSetValues2 = (PFN_SETVALUES2)(g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUES2]);

	hrResult = pfnSetValues2(piThis, eSource, nCount, pnIndices, nStartIndex, ptValues);

	// Keep last status

//lblCleanup:
	return hrResult;
}

static HRESULT regfixhelper_ObtainInterface(
	_In_			IDebugClient *		piClient,
	_COM_Outptr_	IDebugRegisters2 **	ppiDebugRegisters2
	)
{
	HRESULT				hrResult			= E_FAIL;
	IDebugRegisters *	piDebugRegisters	= NULL;
	IDebugRegisters2 *	piDebugRegisters2	= NULL;

	assert(NULL != piClient);
	assert(NULL != ppiDebugRegisters2);

	// COM pointers should be NULL on failure.
	*ppiDebugRegisters2 = NULL;

	hrResult = piClient->lpVtbl->QueryInterface(piClient, &IID_IDebugRegisters, &piDebugRegisters);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	hrResult = piClient->lpVtbl->QueryInterface(piClient, &IID_IDebugRegisters2, &piDebugRegisters2);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	if ((PVOID)(piDebugRegisters->lpVtbl) != (PVOID)(piDebugRegisters2->lpVtbl))
	{
		// TODO: Print warning
	}

	// Transfer ownership:
	*ppiDebugRegisters2 = piDebugRegisters2;
	piDebugRegisters2 = NULL;
	
	hrResult = S_OK;

lblCleanup:
	RELEASE_INTERFACE(piDebugRegisters2);
	RELEASE_INTERFACE(piDebugRegisters);

	return hrResult;
}

static VOID regfixhelper_InitializeGlobalState(
	_In_	IDebugRegisters2 *	piDebugRegisters2
	)
{
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUE] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValue);
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues);
	g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUE] = (FARPROC)(piDebugRegisters2->lpVtbl->SetValue);
	g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUES] = (FARPROC)(piDebugRegisters2->lpVtbl->SetValues);
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES2] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
	g_apfnOriginalFunctions[HOOK_INDEX_SET_VALUES2] = (FARPROC)(piDebugRegisters2->lpVtbl->SetValues2);
}

static VOID regfixhelper_InitializeDescriptors(
	_In_	IDebugRegisters2 *	piDebugRegisters2,
	_Out_	PHOOK_DESCRIPTOR	ptDescriptors
	)
{
	// GetValue
	ptDescriptors[HOOK_INDEX_GET_VALUE].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValue);
	ptDescriptors[HOOK_INDEX_GET_VALUE].pfnHookFunction = (FARPROC)&regfixhelper_GetValueHook;

	// GetValues
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues);
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnHookFunction = (FARPROC)&regfixhelper_GetValuesHook;

	// SetValue
	ptDescriptors[HOOK_INDEX_SET_VALUE].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->SetValue);
	ptDescriptors[HOOK_INDEX_SET_VALUE].pfnHookFunction = (FARPROC)&regfixhelper_SetValueHook;

	// SetValues
	ptDescriptors[HOOK_INDEX_SET_VALUES].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->SetValues);
	ptDescriptors[HOOK_INDEX_SET_VALUES].pfnHookFunction = (FARPROC)&regfixhelper_SetValuesHook;

	// GetValues2
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnHookFunction = (FARPROC)&regfixhelper_GetValues2Hook;

	// SetValues2
	ptDescriptors[HOOK_INDEX_SET_VALUES2].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->SetValues2);
	ptDescriptors[HOOK_INDEX_SET_VALUES2].pfnHookFunction = (FARPROC)&regfixhelper_SetValues2Hook;
}

HRESULT REGFIXHELPER_Prepare(
	_In_								IDebugClient *		piClient,
	_Outptr_result_buffer_(*pnHooks)	PHOOK_DESCRIPTOR *	pptDescriptors,
	_Out_								PDWORD				pnHooks
	)
{
	HRESULT				hrResult			= E_FAIL;
	IDebugRegisters2 *	piDebugRegisters2	= NULL;
	PHOOK_DESCRIPTOR	ptDescriptors		= NULL;

	if ((NULL == piClient) ||
		(NULL == pptDescriptors) ||
		(NULL == pnHooks))
	{
		hrResult = E_INVALIDARG;
		goto lblCleanup;
	}

	hrResult = regfixhelper_ObtainInterface(piClient, &piDebugRegisters2);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	ptDescriptors = HEAPALLOC(HOOKS_COUNT * sizeof(ptDescriptors[0]));
	if (NULL == ptDescriptors)
	{
		hrResult = E_OUTOFMEMORY;
		goto lblCleanup;
	}

	regfixhelper_InitializeGlobalState(piDebugRegisters2);

	regfixhelper_InitializeDescriptors(piDebugRegisters2, ptDescriptors);

	// Transfer ownership:
	*pptDescriptors = ptDescriptors;
	ptDescriptors = NULL;
	*pnHooks = HOOKS_COUNT;

	hrResult = S_OK;

lblCleanup:
	HEAPFREE(ptDescriptors);
	RELEASE_INTERFACE(piDebugRegisters2);

	return hrResult;
}
