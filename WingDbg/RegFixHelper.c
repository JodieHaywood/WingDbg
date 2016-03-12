#include <Windows.h>
#include <DbgEng.h>

#include <assert.h>

#include "Utils.h"
#include "RegFixHooks.h"
#include "RegFixHelper.h"


//
// Functions
//

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
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES2] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
}

static VOID regfixhelper_InitializeDescriptors(
	_In_	IDebugRegisters2 *	piDebugRegisters2,
	_Out_	PHOOK_DESCRIPTOR	ptDescriptors
	)
{
	// GetValue
	ptDescriptors[HOOK_INDEX_GET_VALUE].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValue);
	ptDescriptors[HOOK_INDEX_GET_VALUE].pfnHookFunction = (FARPROC)&REGFIXHOOKS_GetValueHook;

	// GetValues
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues);
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnHookFunction = (FARPROC)&REGFIXHOOKS_GetValuesHook;

	// GetValues2
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnHookFunction = (FARPROC)&REGFIXHOOKS_GetValues2Hook;
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
