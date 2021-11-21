#pragma once

#include <vector>
#include <typeinfo>
#include <string>
#include <chrono>
#include <memory>

#include "Subsystem.h"
#include "Configuration.h"
#include "OptionManager.h"
#include "Logger.h"
#include "Path.h"
#include "Timestamp.h"
#include "Timespan.h"
#include "AutoPtr.h"


class Application : public Subsystem
{
public:
	using ArgVec = std::vector<std::string>;
	using SubsystemPtr = AutoPtr<Subsystem>;
	using SubsystemVec = std::vector<SubsystemPtr>;

	enum ExitCode
	{
		EXIT_OK = 0,
		EXIT_USAGE = 64,
		EXIT_DATAERR = 65,
		EXIT_NOINPUT = 66,
		EXIT_NOUSER = 67,
		EXIT_NOHOST = 68,
		EXIT_UNAVAILABLE = 69,
		EXIT_SOFTWARE = 70,
		EXIT_OSERR = 71,
		EXIT_OSFILE = 72,
		EXIT_CANTCREAT = 73,
		EXIT_IOERR = 74,
		EXIT_TEMPFAIL = 75,
		EXIT_PROTOCOL = 76,
		EXIT_NOPERM = 77,
		EXIT_CONFIG = 78
	};

	enum ConfigPriority
	{
		PRIO_APPLICATION = -100,
		PRIO_DEFAULT = 0,
		PRIO_SYSTEM = 100
	};

	Application();
	Application(int argc, char* argv[]);

	void addSubsystem(Subsystem* pSubsystem);

	void init(int argc, char* argv[]);

	void init(const ArgVec& args);

	bool initialized() const;

	void setUnixOptions(bool flag);

	int loadConfiguration(int priority = PRIO_DEFAULT);

	void loadConfiguration(const std::string& path, int priority = PRIO_DEFAULT);

	template <class C> C& getSubsystem() const;

	SubsystemVec& subsystems();

	virtual int run();

	std::string commandName() const;

	std::string commandPath() const;

	Configuration& config() const;

	Configuration::Ptr configPtr() const;

	Logger& logger() const;

	const ArgVec& argv() const;

	const OptionManager& options() const;

	static Application& instance();

	const Timestamp& startTime() const;

	Timespan uptime() const;

	void stopOptionsProcessing();

	const char* name() const;

protected:
	void initialize(Application& self);

	void uninitialize();

	void reinitialize(Application& self);

	virtual void defineOptions(OptionManager& options);

	virtual void handleOption(const std::string& name, const std::string& value);

	virtual int main(const std::vector<std::string>& args);

	bool findFile(Path& path) const;

	void init();

	~Application();

private:
	void setup();
	void setArgs(int argc, char* argv[]);
	void setArgs(const ArgVec& args);
	void getApplicationPath(Path& path) const;
	void processOptions();
	bool findAppConfigFile(const std::string& appName, const std::string& extension, Path& path) const;
	bool findAppConfigFile(const Path& basePath, const std::string& appName, const std::string& extension, Path& path) const;

private:
	typedef Configuration::Ptr ConfigPtr;
	typedef Logger::Ptr LoggerPtr;

	ConfigPtr       _pConfig;
	SubsystemVec    _subsystems;
	bool            _initialized;

	std::string     _command;
	ArgVec          _argv;
	OptionManager       _options;

	Logger* _pLogger;
	Timestamp _startTime;

private:
	friend class LoggingSubsystem;

	Application(const Application&);
	Application& operator = (const Application&);

	static Application* _pInstance;
};


template <class C> C& Application::getSubsystem() const
{
	for (const auto& pSub : _subsystems)
	{
		const Subsystem* pSS(pSub.get());
		const C* pC = dynamic_cast<const C*>(pSS);
		if (pC) return *const_cast<C*>(pC);
	}
	throw NotFoundException("The subsystem has not been registered", typeid(C).name());
}

inline Application::SubsystemVec& Application::subsystems()
{
	return _subsystems;
}


inline bool Application::initialized() const
{
	return _initialized;
}


inline Configuration& Application::config() const
{
	return *const_cast<Configuration*>(_pConfig.get());
}


inline Configuration::Ptr Application::configPtr() const
{
	return _pConfig;
}


inline Logger& Application::logger() const
{
	CHECK_PTR(_pLogger);
	return *_pLogger;
}


inline const Application::ArgVec& Application::argv() const
{
	return _argv;
}


inline const OptionManager& Application::options() const
{
	return _options;
}


inline Application& Application::instance()
{
	CHECK_PTR(_pInstance);
	return *_pInstance;
}


inline const Timestamp& Application::startTime() const
{
	return _startTime;
}


inline Timespan Application::uptime() const
{
	Timestamp now;
	Timespan uptime = now - _startTime;

	return uptime;
}

