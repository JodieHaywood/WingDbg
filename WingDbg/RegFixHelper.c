#include <Windows.h>
#include <DbgEng.h>

#include <assert.h>

#include "Utils.h"
#include "RegFixHelper.h"


//
// Enums
//

typedef enum _HOOK_INDEX
{
	HOOK_INDEX_GET_VALUE = 0,
	HOOK_INDEX_GET_VALUES,
	HOOK_INDEX_GET_VALUES2,

	// Must be last:
	HOOKS_COUNT
} HOOK_INDEX, *PHOOK_INDEX;


//
// Globals
//

FARPROC g_apfnOriginalFunctions[HOOKS_COUNT] = { NULL };


//
// Typedefs
//

typedef HRESULT(__stdcall * PFN_GETVALUE)(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_Out_	PDEBUG_VALUE		ptValue
	);

typedef HRESULT(__stdcall * PFN_GETVALUES)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);

typedef HRESULT(__stdcall * PFN_GETVALUES2)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);


//
// Functions
//

static HRESULT regfixhelper_HandleGet(
	_In_					IDebugRegisters2 *	piDebugRegisters,
	_In_					ULONG				nCount,
	_In_reads_opt_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	)
{
	HRESULT	hrResult				= E_FAIL;
	ULONG	nBadRegisterIndex		= 0;
	ULONG	nCurrentElement			= 0;
	ULONG	nCurrentRegisterIndex	= 0;

	hrResult = piDebugRegisters->lpVtbl->GetIndexByName(piDebugRegisters,
														"xcr0",
														&nBadRegisterIndex);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	for (nCurrentElement = 0; nCurrentElement < nCount; ++nCurrentElement)
	{
		nCurrentRegisterIndex = (NULL == pnIndices) ? (nCurrentElement + nStartIndex) : (pnIndices[nCurrentElement]);

		if (DEBUG_VALUE_INVALID == ptValues[nCurrentElement].Type)
		{
			if (nCurrentRegisterIndex != nBadRegisterIndex)
			{
				// Function failed on some other register.
				// Be conservative and don't do anything.
				hrResult = E_NOTIMPL;
				goto lblCleanup;
			}
			else
			{
				// This is the bad register.
				// Set it to some valid value.
				ptValues[nCurrentElement].Type = DEBUG_VALUE_INT32;
				ptValues[nCurrentElement].I32 = 0xBADC0DE;
			}
		}
	}

	// Handled!
	hrResult = S_OK;

lblCleanup:
	return hrResult;
}

static HRESULT __stdcall regfixhelper_GetValueHook(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_Out_	PDEBUG_VALUE		ptValue
	)
{
	HRESULT			hrResult	= E_FAIL;
	PFN_GETVALUE	pfnGetValue	= NULL;

	// First, zero the output buffer to parse the results
	// when the original method returns
	SecureZeroMemory(ptValue, sizeof(*ptValue));

	// Invoke the original method
	pfnGetValue = (PFN_GETVALUE)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUE]);
	hrResult = pfnGetValue(piThis, nRegister, ptValue);
	if (E_INVALIDARG == hrResult)
	{
		if (SUCCEEDED(regfixhelper_HandleGet(piThis,
											 1,
											 NULL,
											 nRegister,
											 ptValue)))
		{
			// Handled, so return success to the caller.
			hrResult = S_OK;
		}
	}

	// Keep last status

//lblCleanup:
	return hrResult;
}

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

	// First, zero the output buffer to parse the results
	// when the original method returns
	SecureZeroMemory(ptValues, nCount * sizeof(ptValues[0]));

	// Invoke the original method
	pfnGetValues = (PFN_GETVALUES)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES]);
	hrResult = pfnGetValues(piThis, nCount, pnIndices, nStartIndex, ptValues);
	if (E_INVALIDARG == hrResult)
	{
		if (SUCCEEDED(regfixhelper_HandleGet(piThis,
											nCount,
											pnIndices,
											nStartIndex,
											ptValues)))
		{
			// Handled, so return success to the caller.
			hrResult = S_OK;
		}
	}

	// Keep last status

//lblCleanup:
	return hrResult;
}

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

	// First, zero the output buffer to parse the results
	// when the original method returns
	SecureZeroMemory(ptValues, nCount * sizeof(ptValues[0]));

	// Invoke the original method
	pfnGetValues2 = (PFN_GETVALUES2)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES2]);
	hrResult = pfnGetValues2(piThis, eSource, nCount, pnIndices, nStartIndex, ptValues);
	if (E_INVALIDARG == hrResult)
	{
		if (SUCCEEDED(regfixhelper_HandleGet(piThis,
											 nCount,
											 pnIndices,
											 nStartIndex,
											 ptValues)))
		{
			// Handled, so return success to the caller.
			hrResult = S_OK;
		}
	}

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
		(VOID)UTILS_OutputString(piClient,
								 DEBUG_OUTPUT_EXTENSION_WARNING,
								 "Huh? IDebugRegisters and IDebugRegisters2 are implemented by two separate objects.\n");
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

HRESULT REGFIXHELPER_Initialize(
	_In_								IDebugClient *		piClient,
	_In_								BOOL				bHookSingle,
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

	// GetValue
	if (bHookSingle)
	{
		g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUE] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValue);
		ptDescriptors[HOOK_INDEX_GET_VALUE].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValue);
		ptDescriptors[HOOK_INDEX_GET_VALUE].pfnHookFunction = (FARPROC)&regfixhelper_GetValueHook;
	}

	// GetValues
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues);
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues);
	ptDescriptors[HOOK_INDEX_GET_VALUES].pfnHookFunction = (FARPROC)&regfixhelper_GetValuesHook;

	// GetValues2
	g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES2] = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnFunctionToHook = (FARPROC)(piDebugRegisters2->lpVtbl->GetValues2);
	ptDescriptors[HOOK_INDEX_GET_VALUES2].pfnHookFunction = (FARPROC)&regfixhelper_GetValues2Hook;

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
