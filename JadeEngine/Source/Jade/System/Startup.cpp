#include "./Startup.hpp"

#include "./../Platform/NativeStartup.hpp"

namespace Jade {
	
	void Init() {
		Native::_PlatformInit();
		// TODO: Initialize engine
	}
	
	void Shutdown() {
		// TODO: Shutdown engine
		Native::_PlatformShutdown();
	}
	
}
