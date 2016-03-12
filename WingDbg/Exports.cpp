#include "StdAfx.hpp"

#include "Utils.h"
#include "RegFix.hpp"


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
	UNREFERENCED_PARAMETER(pszArgs);

	try
	{
		WingDbg::Extensions::RegFix(piClient, pszArgs);
	}
	catch (...)
	{
		(void)::UTILS_OutputString(piClient,
								   DEBUG_OUTPUT_ERROR,
								   boost::current_exception_diagnostic_information().c_str());
		return E_FAIL;
	}

	return S_OK;
}
