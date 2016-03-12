#include "StdAfx.hpp"

#include "Utils.h"
#include "Exceptions.hpp"

#include "RegFixHelper.h"


namespace po = boost::program_options;


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

class HookManager final
{
public:
	HookManager(const HookManager & other) = delete;
	HookManager & operator=(const HookManager & other) = delete;
	~HookManager() = default;

	static HookManager & GetInstance()
	{
		static HookManager instance;

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
		CHECK_HRESULT_AND_THROW(::REGFIXHELPER_Initialize(client, &descriptors_unsafe_ptr, &number_descriptors));
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
		DWORD error = hook_library_.Hook(hooks.data(), hooks.size(), NKTHOOKLIB_DisallowReentrancy);
		if (ERROR_SUCCESS != error)
		{
			THROW_WIN32_EXCEPTION(error);
		}

		hooked_ = true;
	}

	void Unhook()
	{
		std::lock_guard<std::mutex> lock(guard_);

		if (!hooked_)
		{
			THROW_COM_EXCEPTION(E_UNEXPECTED);
		}

		hook_library_.UnhookAll();
		hooked_ = false;
	}

private:
	HookManager() :
		hooked_(false)
	{
	}

	std::mutex guard_;
	bool hooked_;
	CNktHookLib hook_library_;
};

}


void RegFix(CComPtr<IDebugClient> client, const std::string & arguments)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "show this help message")
		("unhook,u", "uninstall all hooks")
		;

	po::variables_map vm;
	po::store(po::command_line_parser(po::split_winmain(arguments)).options(desc).run(), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::ostringstream help_message;
		desc.print(help_message);
		(void)::UTILS_OutputString(client, DEBUG_OUTPUT_NORMAL, help_message.str().c_str());
		return;
	}

	if (vm.count("unhook"))
	{
		HookManager::GetInstance().Unhook();
		return;
	}

	HookManager::GetInstance().Hook(client);
}


}
}
