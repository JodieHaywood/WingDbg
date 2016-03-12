#pragma once

#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS

#include <ntstatus.h>
#include <DbgEng.h>
#include <atlbase.h>

#include <cstdarg>
#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/exception/all.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <NktHookLib.h>
