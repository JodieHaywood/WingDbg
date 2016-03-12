#include "StdAfx.hpp"

#include "Utils.h"
#include "Exceptions.hpp"

#include "RegFixHelper.h"


namespace WingDbg {
namespace Extensions {


namespace {

struct ProcessHeapDeleter
{
	void operator()(void * memory)
	{
		::HeapFree(GetProcessHeap(), 0, memory);
	}
};

class RegFixGlobals final
{
public:
	RegFixGlobals(const RegFixGlobals & other) = delete;
	RegFixGlobals & operator=(const RegFixGlobals & other) = delete;
	~RegFixGlobals() = default;

	static RegFixGlobals & GetInstance()
	{
		static RegFixGlobals instance;

		return instance;
	}

	void Hook(CComPtr<IDebugClient> client)
	{
		std::lock_guard<std::mutex> lock(guard_);

		if (hooked_)
		{
			THROW_COM_EXCEPTION(E_UNEXPECTED);
		}

		// Initialize the low-level state and obtain
		// the addresses of the hook functions.
		PHOOK_DESCRIPTOR descriptors_unsafe_ptr = nullptr;
		DWORD number_descriptors = 0;
		CHECK_HRESULT_AND_THROW(::REGFIXHELPER_Prepare(client, &descriptors_unsafe_ptr, &number_descriptors));
		std::unique_ptr<HOOK_DESCRIPTOR[], decltype(ProcessHeapDeleter())> descriptors(descriptors_unsafe_ptr, ProcessHeapDeleter());

		// Initialize the array to be passed to the hooking framework.
		std::vector<CNktHookLib::HOOK_INFO> hooks(number_descriptors);
		for (DWORD index = 0; index < number_descriptors; ++index)
		{
			hooks[index].nHookId = index;
			hooks[index].lpProcToHook = (PVOID)(DWORD_PTR)(descriptors[index].pfnFunctionToHook);
			hooks[index].lpNewProcAddr = (PVOID)(DWORD_PTR)(descriptors[index].pfnHookFunction);
		}

		// I love to hook it, hook it!
		DWORD error = hook_manager_.Hook(hooks.data(), hooks.size(), NKTHOOKLIB_DisallowReentrancy);
		if (ERROR_SUCCESS != error)
		{
			THROW_WIN32_EXCEPTION(error);
		}

		hooked_ = true;
	}

private:
	RegFixGlobals() :
		hooked_(false)
	{
	}

	std::mutex guard_;
	bool hooked_;
	CNktHookLib hook_manager_;
};

}


void RegFix(CComPtr<IDebugClient> client, const std::string & arguments)
{
	UNREFERENCED_PARAMETER(arguments);

	RegFixGlobals::GetInstance().Hook(client);
}


}
}
