#pragma once

#include "StdAfx.hpp"


#define THROW_COM_EXCEPTION(hrResult) \
	BOOST_THROW_EXCEPTION(Exceptions::WingDbgException() << Exceptions::ComErrorInfo(hrResult))

#define THROW_WIN32_EXCEPTION(dwError) \
	BOOST_THROW_EXCEPTION(Exceptions::WingDbgException() << Exceptions::Win32ErrorInfo(dwError))

#define THROW_NT_EXCEPTION(eNtStatus) \
	BOOST_THROW_EXCEPTION(Exceptions::WingDbgException() << Exceptions::NtErrorInfo(eNtStatus))

#define CHECK_HRESULT_AND_THROW(hrResult)		\
	do											\
	{											\
		HRESULT hrResultTemp = (hrResult);		\
		if (FAILED(hrResultTemp))				\
		{										\
			THROW_COM_EXCEPTION(hrResultTemp);	\
		}										\
	} while (0)


namespace WingDbg {
namespace Exceptions {



typedef boost::error_info<struct ComErrorInfo_, HRESULT> ComErrorInfo;
std::string to_string(const ComErrorInfo & com_error_info);

typedef boost::error_info<struct Win32ErrorInfo_, DWORD> Win32ErrorInfo;
std::string to_string(const Win32ErrorInfo & win32_error_info);

typedef boost::error_info<struct NtErrorInfo_, NTSTATUS> NtErrorInfo;
std::string to_string(const NtErrorInfo & nt_error_info);

typedef boost::error_info<struct ErrorMessage_, const char *> ErrorMessage;

struct WingDbgException : virtual boost::exception, virtual std::exception
{
};


}
}
