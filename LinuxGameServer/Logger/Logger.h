#pragma once

#include "ILogger.h"
#include "Format.h"
#include "AutoPtr.h"

#include <map>
#include <vector>
#include <cstddef>
#include <memory>
#include <mutex>

class Exception;


class Logger : public ILogger
{
public:
	using Ptr = Logger*;

public:	
	const std::string& name() const;

	void setLevel(int level);
	void setLevel(const std::string& level);

	int getLevel() const;

	void setType(int type);
	int getType() const;

public:
	void log(const Exception& exc);
	void log(const Exception& exc, const char* file, int line);

	void fatal(const std::string& msg);
	void fatal(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void fatal(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Fatal);
	}

	void critical(const std::string& msg);
	void critical(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void critical(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Critical);
	}

	void error(const std::string& msg);
	void error(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void error(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Error);
	}

	void warning(const std::string& msg);
	void warning(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void warning(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Warning);
	}

	void notice(const std::string& msg);
	void notice(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void notice(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Notice);
	}

	void information(const std::string& msg);
	void information(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void information(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Information);
	}

	void debug(const std::string& msg);
	void debug(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void debug(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Debug);
	}

	void trace(const std::string& msg);
	void trace(const std::string& msg, const char* file, int line);

	template <typename T, typename... Args>
	void trace(const std::string& fmt, T arg1, Args&&... args)
	{
		log(format(fmt, arg1, std::forward<Args>(args)...), LogPrio_Trace);
	}

	void dump(const std::string& msg, const void* buffer, std::size_t length, LogPriority prio = LogPrio_Debug);

public:
	bool is(int level) const;

	bool fatal() const;
	bool critical() const;
	bool error() const;
	bool warning() const;
	bool notice() const;
	bool information() const;
	bool debug() const;
	bool trace() const;

public:
	static std::string format(const std::string& fmt, const std::string& arg);

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1);

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2);

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3);

	static void formatDump(std::string& message, const void* buffer, std::size_t length);

	static int parseLevel(const std::string& level);

public:
	static void setLevel(const std::string& name, int level);

	static void setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value);

	static Logger& get(const std::string& name);

	static Logger& unsafeGet(const std::string& name);

	static Logger& create(const std::string& name, int type = LogType_Console, int level = LogPrio_Information);

	static Logger& root();

	static Ptr has(const std::string& name);

	static void destroy(const std::string& name);

	static void shutdown();

	static void names(std::vector<std::string>& names);

	static Logger& parent(const std::string& name);
	static void add(Ptr pLogger);
	static Ptr find(const std::string& name);

protected:
	Logger(const std::string& name, int level, int type);
	~Logger();

	void log(const std::string& text, LogPriority prio);
	void log(const std::string& text, LogPriority prio, const char* file, int line);

	static std::string format(const std::string& fmt, int argc, std::string argv[]);

private:
	Logger();
	Logger(const Logger&);
	Logger& operator = (const Logger&);

	std::string _name;
	int         _level;
	int         _type;

	static const std::string ROOT;

protected:
	typedef std::map<std::string, Ptr> LoggerMap;
	typedef std::unique_ptr<LoggerMap> LoggerMapPtr;

	static LoggerMapPtr	_pLoggerMap;
	static std::mutex	_mapMtx;
};


//
// inlines
//

inline const std::string& Logger::name() const
{
	return _name;
}

inline int Logger::getLevel() const
{
	return _level;
}

inline void Logger::setType(int type)
{
	_type = type;
}

inline int Logger::getType() const
{
	return _type;
}

inline void Logger::fatal(const std::string& msg)
{
	log(msg, LogPrio_Fatal);
}


inline void Logger::fatal(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Fatal, file, line);
}



inline void Logger::critical(const std::string& msg)
{
	log(msg, LogPrio_Critical);
}


inline void Logger::critical(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Critical, file, line);
}


inline void Logger::error(const std::string& msg)
{
	log(msg, LogPrio_Error);
}


inline void Logger::error(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Error, file, line);
}


inline void Logger::warning(const std::string& msg)
{
	log(msg, LogPrio_Warning);
}


inline void Logger::warning(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Warning, file, line);
}


inline void Logger::notice(const std::string& msg)
{
	log(msg, LogPrio_Notice);
}


inline void Logger::notice(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Notice, file, line);
}


inline void Logger::information(const std::string& msg)
{
	log(msg, LogPrio_Information);
}


inline void Logger::information(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Information, file, line);
}


inline void Logger::debug(const std::string& msg)
{
	log(msg, LogPrio_Debug);
}


inline void Logger::debug(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Debug, file, line);
}


inline void Logger::trace(const std::string& msg)
{
	log(msg, LogPrio_Trace);
}


inline void Logger::trace(const std::string& msg, const char* file, int line)
{
	log(msg, LogPrio_Trace, file, line);
}


inline bool Logger::is(int level) const
{
	return _level >= level;
}


inline bool Logger::fatal() const
{
	return _level >= LogPrio_Fatal;
}


inline bool Logger::critical() const
{
	return _level >= LogPrio_Critical;
}


inline bool Logger::error() const
{
	return _level >= LogPrio_Error;
}


inline bool Logger::warning() const
{
	return _level >= LogPrio_Warning;
}


inline bool Logger::notice() const
{
	return _level >= LogPrio_Notice;
}


inline bool Logger::information() const
{
	return _level >= LogPrio_Information;
}


inline bool Logger::debug() const
{
	return _level >= LogPrio_Debug;
}


inline bool Logger::trace() const
{
	return _level >= LogPrio_Trace;
}

