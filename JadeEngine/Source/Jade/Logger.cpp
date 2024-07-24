#include "./Logger.hpp"

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include "./System/Memory.hpp"

namespace Jade {
	
	Logger::Logger(const char *name, LogSeverity severity) : m_Name(name), m_Severity(severity) { }
	
	void Logger::Trace(const char *message, ...) {
		if (m_Severity < LogSeverity::Trace) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		char *formattedMessage = new char[size];
		(void)vsprintf(formattedMessage, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		printf("\033[95m[%d:%d:%d] [%s] TRACE > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
	void Logger::Info(const char *message, ...) {
		if (m_Severity < LogSeverity::Info) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		char *formattedMessage = new char[size];
		vsprintf(formattedMessage, message, list);
		
		printf("\033[94m[%d:%d:%d] [%s] INFO > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
	void Logger::Debug(const char *message, ...) {
		if (m_Severity < LogSeverity::Debug) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		char *formattedMessage = new char[size];
		(void)vsprintf(formattedMessage, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		printf("\033[92m[%d:%d:%d] [%s] DEBUG > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
	void Logger::Warn(const char *message, ...) {
		if (m_Severity < LogSeverity::Warning) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		char *formattedMessage = new char[size];
		(void)vsprintf(formattedMessage, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		printf("\033[93m[%d:%d:%d] [%s] WARNING > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
	void Logger::Error(const char *message, ...) {
		if (m_Severity < LogSeverity::Error) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		char *formattedMessage = new char[size];
		(void)vsprintf(formattedMessage, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		printf("\033[31m[%d:%d:%d] [%s] ERROR > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
	void Logger::Fatal(const char *message, ...) {
		if (m_Severity < LogSeverity::Fatal) return;
		std::va_list list;
		va_start(list, message);
		
		int size = vsnprintf(nullptr, 0, message, list);
		
		char *formattedMessage = new char[size];
		(void)vsprintf(formattedMessage, message, list);
		
		time_t rawTime = 0;
		time(&rawTime);
		
		struct tm *time = localtime(&rawTime);
		
		printf("\033[97m\033[41m[%d:%d:%d] [%s] FATAL > %.*s\033[0m\n", time->tm_hour, time->tm_min, time->tm_sec, m_Name, size, formattedMessage); 
		
		// delete[] formattedMessage;
		va_end(list);
	}
	
}