#include <Windows.h>
#include <DbgEng.h>

#include "RegFixHooks.h"


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

static HRESULT regfixhooks_HandleGet(
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

HRESULT __stdcall REGFIXHOOKS_GetValueHook(
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

HRESULT __stdcall REGFIXHOOKS_GetValuesHook(
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
		if (SUCCEEDED(regfixhooks_HandleGet(piThis,
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

HRESULT __stdcall REGFIXHOOKS_GetValues2Hook(
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
