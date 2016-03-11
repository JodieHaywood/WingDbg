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

typedef HRESULT(__stdcall * PFN_SETVALUE)(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_In_	PDEBUG_VALUE		ptValue
	);

typedef HRESULT(__stdcall * PFN_SETVALUES)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);

typedef HRESULT(__stdcall * PFN_GETVALUES2)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);

typedef HRESULT(__stdcall * PFN_SETVALUES2)(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);


//
// Functions
//

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

	pfnGetValues = (PFN_GETVALUES)(g_apfnOriginalFunctions[HOOK_INDEX_GET_VALUES]);

	hrResult = pfnGetValues(piThis, nCount, pnIndices, nStartIndex, ptValues);

	// Keep last status

//lblCleanup:
	return hrResult;
}

HRESULT __stdcall REGFIXHOOKS_SetValueHook(
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

HRESULT __stdcall REGFIXHOOKS_SetValuesHook(
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

HRESULT __stdcall REGFIXHOOKS_SetValues2Hook(
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
