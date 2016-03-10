#pragma once

#include "StdAfx.hpp"


namespace WingDbg {
namespace Utils {


void OutputString(CComPtr<IDebugClient> client, ULONG mask, PCSTR format, ...) noexcept;


}
}
