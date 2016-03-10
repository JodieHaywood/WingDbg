#include "StdAfx.hpp"

#include "Exceptions.hpp"


namespace WingDbg {
namespace Exceptions {


namespace {

std::string FormatSystemMessage(DWORD message_id)
{
	// ntdll should always be loaded for the lifetime
	// of the process. No need to bump the reference count.
	HMODULE ntdll_handle = ::GetModuleHandleW(L"ntdll.dll");
	if (NULL == ntdll_handle)
	{
		THROW_WIN32_EXCEPTION(GetLastError());
	}

	PSTR message_unsafe_ptr = nullptr;
	DWORD result = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
									ntdll_handle,
									message_id,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									reinterpret_cast<PSTR>(&message_unsafe_ptr),
									0,
									nullptr);
	std::unique_ptr<CHAR, HLOCAL(WINAPI *)(HLOCAL)> message_ptr(message_unsafe_ptr, &::LocalFree);
	if (0 == result)
	{
		return std::string();
	}

	std::string message(message_ptr.get());
	boost::trim(message);

	return message;
}

}



std::string to_string(const ComErrorInfo & com_error_info)
{
	std::ostringstream temp;

	temp << "COM error: 0x" << std::hex << com_error_info.value();

	auto message = FormatSystemMessage(com_error_info.value());
	if (!message.empty())
	{
		temp << ", \"" << message << "\"";
	}

	temp << std::endl;

	return temp.str();
}

std::string to_string(const Win32ErrorInfo & win32_error_info)
{
	std::ostringstream temp;

	temp << "Win32 error: " << win32_error_info.value();

	auto message = FormatSystemMessage(win32_error_info.value());
	if (!message.empty())
	{
		temp << ", \"" << message << "\"";
	}

	temp << std::endl;

	return temp.str();
}

std::string to_string(const NtErrorInfo & nt_error_info)
{
	std::ostringstream temp;

	temp << "NTSTATUS: 0x" << std::hex << nt_error_info.value();

	auto message = FormatSystemMessage(nt_error_info.value());
	if (!message.empty())
	{
		temp << ", \"" << message << "\"";
	}

	temp << std::endl;

	return temp.str();
}


}
}