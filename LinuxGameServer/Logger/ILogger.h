#pragma once

#include <string>
#include "BaseException.h"

/// <summary>
/// 日志等级
/// </summary>
enum LogPriority
{
	LogPrio_Fatal = 1,
	LogPrio_Critical,
	LogPrio_Error,
	LogPrio_Warning,
	LogPrio_Notice,
	LogPrio_Information,
	LogPrio_Debug,
	LogPrio_Trace
};

/// <summary>
///	日志类型
/// </summary>
enum LogType
{
	LogType_Console,
	LogType_File,
	LogType_SysLog,
	LogType_SMTP,
};


class ILogger
{
public:
	virtual ~ILogger() {}

	virtual const std::string& name() const = 0;

	virtual int getLevel() const = 0;

	virtual int getType() const = 0;

public:
	virtual void log(const Exception& exc) = 0;
	virtual void log(const Exception& exc, const char* file, int line) = 0;

	virtual void fatal(const std::string& msg) = 0;
	virtual void fatal(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void fatal(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Fatal);
	}

	virtual void critical(const std::string& msg) = 0;
	virtual void critical(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void critical(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Critical);
	}

	virtual void error(const std::string& msg) = 0;
	virtual void error(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void error(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Error);
	}

	virtual void warning(const std::string& msg) = 0;
	virtual void warning(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void warning(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Warning);
	}

	virtual void notice(const std::string& msg) = 0;
	virtual void notice(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void notice(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Notice);
	}

	virtual void information(const std::string& msg) = 0;
	virtual void information(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void information(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Information);
	}

	virtual void debug(const std::string& msg) = 0;
	virtual void debug(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void debug(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Debug);
	}

	virtual void trace(const std::string& msg) = 0;
	virtual void trace(const std::string& msg, const char* file, int line) = 0;
	template <typename T, typename... Args>
	void trace(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Trace);
	}

	virtual void dump(const std::string& msg, const void* buffer, std::size_t length, LogPriority prio = LogPrio_Debug) = 0;
};


//
// convenience macros
//
#define log_fatal(logger, msg) \
	if ((logger).fatal()) (logger).fatal(msg, __FILE__, __LINE__); else (void) 0

#define log_fatal_f1(logger, fmt, arg1) \
	if ((logger).fatal()) (logger).fatal(format((fmt), arg1), __FILE__, __LINE__); else (void) 0

#define log_fatal_f2(logger, fmt, arg1, arg2) \
	if ((logger).fatal()) (logger).fatal(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_fatal_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).fatal()) (logger).fatal(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_fatal_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).fatal()) (logger).fatal(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_fatal_f(logger, fmt, ...) \
	if ((logger).fatal()) (logger).fatal(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_critical(logger, msg) \
	if ((logger).critical()) (logger).critical(msg, __FILE__, __LINE__); else (void) 0

#define log_critical_f1(logger, fmt, arg1) \
	if ((logger).critical()) (logger).critical(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_critical_f2(logger, fmt, arg1, arg2) \
	if ((logger).critical()) (logger).critical(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_critical_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).critical()) (logger).critical(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_critical_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).critical()) (logger).critical(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_critical_f(logger, fmt, ...) \
	if ((logger).critical()) (logger).critical(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_error(logger, msg) \
	if ((logger).error()) (logger).error(msg, __FILE__, __LINE__); else (void) 0

#define log_error_f1(logger, fmt, arg1) \
	if ((logger).error()) (logger).error(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_error_f2(logger, fmt, arg1, arg2) \
	if ((logger).error()) (logger).error(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_error_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).error()) (logger).error(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_error_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).error()) (logger).error(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_error_f(logger, fmt, ...) \
	if ((logger).error()) (logger).error(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_warning(logger, msg) \
	if ((logger).warning()) (logger).warning(msg, __FILE__, __LINE__); else (void) 0

#define log_warning_f1(logger, fmt, arg1) \
	if ((logger).warning()) (logger).warning(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_warning_f2(logger, fmt, arg1, arg2) \
	if ((logger).warning()) (logger).warning(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_warning_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).warning()) (logger).warning(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_warning_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).warning()) (logger).warning(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_warning_f(logger, fmt, ...) \
	if ((logger).warning()) (logger).warning(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_notice(logger, msg) \
	if ((logger).notice()) (logger).notice(msg, __FILE__, __LINE__); else (void) 0

#define log_notice_f1(logger, fmt, arg1) \
	if ((logger).notice()) (logger).notice(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_notice_f2(logger, fmt, arg1, arg2) \
	if ((logger).notice()) (logger).notice(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_notice_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).notice()) (logger).notice(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_notice_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).notice()) (logger).notice(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_notice_f(logger, fmt, ...) \
	if ((logger).notice()) (logger).notice(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_information(logger, msg) \
	if ((logger).information()) (logger).information(msg, __FILE__, __LINE__); else (void) 0

#define log_information_f1(logger, fmt, arg1) \
	if ((logger).information()) (logger).information(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_information_f2(logger, fmt, arg1, arg2) \
	if ((logger).information()) (logger).information(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_information_f3(logger, fmt, arg1, arg2, arg3) \
	if ((logger).information()) (logger).information(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_information_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	if ((logger).information()) (logger).information(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_information_f(logger, fmt, ...) \
	if ((logger).information()) (logger).information(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#if defined(_DEBUG) || defined(_LOG_DEBUG)
#define log_debug(logger, msg) \
		if ((logger).debug()) (logger).debug(msg, __FILE__, __LINE__); else (void) 0

#define log_debug_f1(logger, fmt, arg1) \
		if ((logger).debug()) (logger).debug(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_debug_f2(logger, fmt, arg1, arg2) \
		if ((logger).debug()) (logger).debug(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_debug_f3(logger, fmt, arg1, arg2, arg3) \
		if ((logger).debug()) (logger).debug(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_debug_f4(logger, fmt, arg1, arg2, arg3, arg4) \
		if ((logger).debug()) (logger).debug(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_debug_f(logger, fmt, ...) \
		if ((logger).debug()) (logger).debug(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0

#define log_trace(logger, msg) \
		if ((logger).trace()) (logger).trace(msg, __FILE__, __LINE__); else (void) 0

#define log_trace_f1(logger, fmt, arg1) \
		if ((logger).trace()) (logger).trace(format((fmt), (arg1)), __FILE__, __LINE__); else (void) 0

#define log_trace_f2(logger, fmt, arg1, arg2) \
		if ((logger).trace()) (logger).trace(format((fmt), (arg1), (arg2)), __FILE__, __LINE__); else (void) 0

#define log_trace_f3(logger, fmt, arg1, arg2, arg3) \
		if ((logger).trace()) (logger).trace(format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); else (void) 0

#define log_trace_f4(logger, fmt, arg1, arg2, arg3, arg4) \
		if ((logger).trace()) (logger).trace(format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); else (void) 0

#define log_trace_f(logger, fmt, ...) \
		if ((logger).trace()) (logger).trace(format((fmt), __VA_ARGS__), __FILE__, __LINE__); else (void) 0
#else
#define log_debug(logger, msg)
#define log_debug_f1(logger, fmt, arg1)
#define log_debug_f2(logger, fmt, arg1, arg2)
#define log_debug_f3(logger, fmt, arg1, arg2, arg3)
#define log_debug_f4(logger, fmt, arg1, arg2, arg3, arg4)
#define log_debug_f(logger, fmt, ...)
#define log_trace(logger, msg)
#define log_trace_f1(logger, fmt, arg1)
#define log_trace_f2(logger, fmt, arg1, arg2)
#define log_trace_f3(logger, fmt, arg1, arg2, arg3)
#define log_trace_f4(logger, fmt, arg1, arg2, arg3, arg4)
#define log_trace_f(logger, fmt, ...)
#endif
