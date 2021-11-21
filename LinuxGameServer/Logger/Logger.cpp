#include "Logger.h"
#include "BaseException.h"
#include "NumberFormatter.h"
#include "NumberParser.h"
#include "TString.h"

Logger::LoggerMapPtr	Logger::_pLoggerMap;
std::mutex				Logger::_mapMtx;
const std::string		Logger::ROOT;

Logger::Logger(const std::string& name, int level, int type) :
	_name(name),
	_level(level),
	_type(type)
{
}


Logger::~Logger()
{
}


void Logger::setLevel(int level)
{
	_level = level;
}


void Logger::setLevel(const std::string& level)
{
	setLevel(parseLevel(level));
}


void Logger::log(const Exception& exc)
{
	error(exc.displayText());
}


void Logger::log(const Exception& exc, const char* file, int line)
{
	error(exc.displayText(), file, line);
}


void Logger::dump(const std::string& msg, const void* buffer, std::size_t length, LogPriority prio)
{
	if (_level >= prio)
	{
		std::string text(msg);
		formatDump(text, buffer, length);
		//_pChannel->log(Message(_name, text, prio));
	}
}



void Logger::log(const std::string& text, LogPriority prio)
{
	if (_level >= prio)
	{
		//_pChannel->log(Message(_name, text, prio));
	}
}


void Logger::log(const std::string& text, LogPriority prio, const char* file, int line)
{
	if (_level >= prio)
	{
		//_pChannel->log(Message(_name, text, prio, file, line));
	}
}


std::string Logger::format(const std::string& fmt, const std::string& arg)
{
	std::string args[] =
	{
		arg
	};
	return format(fmt, 1, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1)
{
	std::string args[] =
	{
		arg0,
		arg1
	};
	return format(fmt, 2, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2)
{
	std::string args[] =
	{
		arg0,
		arg1,
		arg2
	};
	return format(fmt, 3, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3)
{
	std::string args[] =
	{
		arg0,
		arg1,
		arg2,
		arg3
	};
	return format(fmt, 4, args);
}


std::string Logger::format(const std::string& fmt, int argc, std::string argv[])
{
	std::string result;
	std::string::const_iterator it = fmt.begin();
	while (it != fmt.end())
	{
		if (*it == '$')
		{
			++it;
			if (*it == '$')
			{
				result += '$';
			}
			else if (*it >= '0' && *it <= '9')
			{
				int i = *it - '0';
				if (i < argc)
					result += argv[i];
			}
			else
			{
				result += '$';
				result += *it;
			}
		}
		else result += *it;
		++it;
	}
	return result;
}


void Logger::formatDump(std::string& message, const void* buffer, std::size_t length)
{
	const int BYTES_PER_LINE = 16;

	message.reserve(message.size() + length * 6);
	if (!message.empty()) message.append("\n");
	unsigned char* base = (unsigned char*)buffer;
	std::size_t addr = 0;
	while (addr < length)
	{
		if (addr > 0) message.append("\n");
		message.append(NumberFormatter::formatHex(addr, 4));
		message.append("  ");
		std::size_t offset = 0;
		while (addr + offset < length && offset < BYTES_PER_LINE)
		{
			message.append(NumberFormatter::formatHex(base[addr + offset], 2));
			message.append(offset == 7 ? "  " : " ");
			++offset;
		}
		if (offset < 7) message.append(" ");
		while (offset < BYTES_PER_LINE) { message.append("   "); ++offset; }
		message.append(" ");
		offset = 0;
		while (addr + offset < length && offset < BYTES_PER_LINE)
		{
			unsigned char c = base[addr + offset];
			message += (c >= 32 && c < 127) ? (char)c : '.';
			++offset;
		}
		addr += BYTES_PER_LINE;
	}
}

int Logger::parseLevel(const std::string& level)
{
	if (icompare(level, "none") == 0)
		return 0;
	else if (icompare(level, "fatal") == 0)
		return LogPrio_Fatal;
	else if (icompare(level, "critical") == 0)
		return LogPrio_Critical;
	else if (icompare(level, "error") == 0)
		return LogPrio_Error;
	else if (icompare(level, "warning") == 0)
		return LogPrio_Warning;
	else if (icompare(level, "notice") == 0)
		return LogPrio_Notice;
	else if (icompare(level, "information") == 0)
		return LogPrio_Information;
	else if (icompare(level, "debug") == 0)
		return LogPrio_Debug;
	else if (icompare(level, "trace") == 0)
		return LogPrio_Trace;
	else
	{
		int numLevel;
		if (NumberParser::tryParse(level, numLevel))
		{
			if (numLevel > 0 && numLevel < 9)
				return numLevel;
			else
				throw InvalidArgumentException("Log level out of range ", level);
		}
		else
			throw InvalidArgumentException("Not a valid log level", level);
	}
}

void Logger::setLevel(const std::string& name, int level)
{
	std::unique_lock<std::mutex> lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string::size_type len = name.length();
		for (auto& p : *_pLoggerMap)
		{
			if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				p.second->setLevel(level);
			}
		}
	}
}

Logger& Logger::get(const std::string& name)
{
	std::unique_lock<std::mutex> lock(_mapMtx);
	return unsafeGet(name);
}

Logger& Logger::unsafeGet(const std::string& name)
{
	Ptr pLogger = find(name);
	if (!pLogger)
	{
		if (name == ROOT)
		{
			pLogger = new Logger(name, LogType_Console, LogPrio_Information);
		}
		else
		{
			Logger& par = parent(name);
			pLogger = new Logger(name, par.getType(), par.getLevel());
		}
		add(pLogger);
	}
	return *pLogger;
}

Logger& Logger::root()
{
	std::unique_lock<std::mutex> lock(_mapMtx);
	return unsafeGet(ROOT);
}

Logger::Ptr Logger::has(const std::string& name)
{
	std::unique_lock<std::mutex> lock(_mapMtx);
	return find(name);
}

void Logger::destroy(const std::string& name)
{
	std::unique_lock<std::mutex> lock(_mapMtx);

	if (_pLoggerMap)
	{
		LoggerMap::iterator it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end()) _pLoggerMap->erase(it);
	}
}

void Logger::shutdown()
{
	std::unique_lock<std::mutex> lock(_mapMtx);
	_pLoggerMap.reset();
}

void Logger::names(std::vector<std::string>& names)
{
	std::unique_lock<std::mutex> lock(_mapMtx);

	names.clear();
	if (_pLoggerMap)
	{
		for (const auto& p : *_pLoggerMap)
		{
			names.push_back(p.first);
		}
	}
}

Logger& Logger::parent(const std::string& name)
{
	std::string::size_type pos = name.rfind('.');
	if (pos != std::string::npos)
	{
		std::string pname = name.substr(0, pos);
		Ptr pParent = find(pname);
		if (pParent)
			return *pParent;
		else
			return parent(pname);
	}
	else return unsafeGet(ROOT);
}

void Logger::add(Ptr pLogger)
{
	if (!_pLoggerMap) _pLoggerMap.reset(new LoggerMap);
	_pLoggerMap->insert(LoggerMap::value_type(pLogger->name(), pLogger));
}

Logger::Ptr Logger::find(const std::string& name)
{
	if (_pLoggerMap)
	{
		LoggerMap::iterator it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end()) return it->second;
	}
	return 0;
}

