#include "StdAfx.hpp"

#include "Utils.hpp"


HRESULT CALLBACK DebugExtensionInitialize(
	_Out_	PULONG	pnVersion,
	_Out_	PULONG	pfFlags
	)
{
	UNREFERENCED_PARAMETER(pfFlags);

	if (NULL == pnVersion)
	{
		return E_INVALIDARG;
	}

	*pnVersion = DEBUG_EXTENSION_VERSION(1, 0);

	return S_OK;
}

HRESULT CALLBACK regfix(
	_In_		PDEBUG_CLIENT	piClient,
	_In_opt_	PCSTR			pszArgs
	)
{
	UNREFERENCED_PARAMETER(piClient);
	UNREFERENCED_PARAMETER(pszArgs);

	return S_OK;
}
