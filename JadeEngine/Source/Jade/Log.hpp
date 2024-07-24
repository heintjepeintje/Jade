#pragma once

#include "./Logger.hpp"

namespace Jade {

	Logger GetCoreLogger();
	Logger GetClientLogger();

}

#if defined(_DEBUG)
#	define JD_TRACE(...) ::Jade::GetCoreLogger().Trace(__VA_ARGS__)
#	define JD_INFO(...) ::Jade::GetCoreLogger().Info(__VA_ARGS__)
#	define JD_DEBUG(...) ::Jade::GetCoreLogger().Debug(__VA_ARGS__)
#	define JD_WARN(...) ::Jade::GetCoreLogger().Warn(__VA_ARGS__)
#	define JD_ERROR(...) ::Jade::GetCoreLogger().Error(__VA_ARGS__)
#	define JD_FATAL(...) ::Jade::GetCoreLogger().Fatal(__VA_ARGS__)
#else
#	define JD_TRACE(...)
#	define JD_INFO(...)
#	define JD_DEBUG(...)
#	define JD_WARN(...)
#	define JD_ERROR(...)
#	define JD_FATAL(...)
#endif