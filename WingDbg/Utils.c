#include <Windows.h>
#include <DbgEng.h>

#include <stdarg.h>

#include "Utils.h"


HRESULT OutputString(
	_In_	IDebugClient *	piClient,
	_In_	ULONG			nMask,
	_In_	PCSTR			pszFormat,
	...
	)
{
	HRESULT			hrResult			= E_FAIL;
	IDebugControl *	piDebugControl		= NULL;
	va_list			vaArguments			= NULL;
	BOOL			bArgsInitialized	= FALSE;

	hrResult = piClient->lpVtbl->QueryInterface(piClient,
												&IID_IDebugControl,
												&piDebugControl);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	va_start(vaArguments, pszFormat);
	bArgsInitialized = TRUE;

	hrResult = piDebugControl->lpVtbl->OutputVaList(piDebugControl,
													nMask,
													pszFormat,
													vaArguments);
	if (FAILED(hrResult))
	{
		goto lblCleanup;
	}

	hrResult = S_OK;

lblCleanup:
	if (bArgsInitialized)
	{
		va_end(vaArguments);
		bArgsInitialized = FALSE;
	}
	if (NULL != piDebugControl)
	{
		(VOID)(piDebugControl->lpVtbl->Release(piDebugControl));
		piDebugControl = NULL;
	}

	return hrResult;
}
