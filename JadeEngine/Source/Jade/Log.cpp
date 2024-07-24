#include "./Log.hpp"

namespace Jade {
	
	static Logger s_CoreLogger = Logger("JADE");
	static Logger s_ClientLogger = Logger("APP");
	
	Logger GetCoreLogger() {
		return s_CoreLogger;
	}
	
	Logger GetClientLogger() {
		return s_ClientLogger;
	}
	
}