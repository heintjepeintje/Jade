#pragma once

namespace Jade {

	enum struct LogSeverity {
		None = 0,
		Fatal, Error, Warning,
		Debug, Info, Trace,
		All
	};

	class Logger {
	public:
		Logger() = default;
		Logger(const char *name, LogSeverity severity = LogSeverity::All);
		~Logger() = default;
		
		inline void SetSeverity(LogSeverity severity) { m_Severity = severity; }
		inline LogSeverity GetSeverity() const { return m_Severity; }
		inline const char *GetName() const { return m_Name; }
		
		void Trace(const char *message, ...);
		void Info(const char *message, ...);
		void Debug(const char *message, ...);
		void Warn(const char *message, ...);
		void Error(const char *message, ...);
		void Fatal(const char *message, ...);
		
	private:
		const char *m_Name;
		LogSeverity m_Severity;
	};
	
}