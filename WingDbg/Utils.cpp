#include "StdAfx.hpp"

#include "Exceptions.hpp"
#include "Utils.hpp"


namespace WingDbg {
namespace Utils {


void OutputString(CComPtr<IDebugClient> client, ULONG mask, PCSTR format, ...) noexcept
{
	CComPtr<IDebugControl> debug_control;
	if (SUCCEEDED(client->QueryInterface(IID_PPV_ARGS(&debug_control))))
	{
		std::va_list arguments;
		va_start(arguments, format);
		(void)debug_control->OutputVaList(mask, format, arguments);
		va_end(arguments);
	}
}


}
}
