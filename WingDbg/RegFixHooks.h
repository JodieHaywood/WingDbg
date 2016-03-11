#pragma once

#include <Windows.h>
#include <DbgEng.h>


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

FARPROC g_apfnOriginalFunctions[HOOKS_COUNT];


//
// Functions
//

HRESULT __stdcall REGFIXHOOKS_GetValueHook(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_Out_	PDEBUG_VALUE		ptValue
	);

HRESULT __stdcall REGFIXHOOKS_GetValuesHook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);

HRESULT __stdcall REGFIXHOOKS_SetValueHook(
	_Inout_	IDebugRegisters2 *	piThis,
	_In_	ULONG				nRegister,
	_In_	PDEBUG_VALUE		ptValue
	);

HRESULT __stdcall REGFIXHOOKS_SetValuesHook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);

HRESULT __stdcall REGFIXHOOKS_GetValues2Hook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_Out_writes_(nCount)	PDEBUG_VALUE		ptValues
	);

HRESULT __stdcall REGFIXHOOKS_SetValues2Hook(
	_Inout_					IDebugRegisters2 *	piThis,
	_In_					ULONG				eSource,
	_In_					ULONG				nCount,
	_In_opt_count_(nCount)	PULONG				pnIndices,
	_In_					ULONG				nStartIndex,
	_In_reads_(nCount)		PDEBUG_VALUE		ptValues
	);

