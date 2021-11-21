#include "Application.h"
#include "NumberFormatter.h"
#include "File.h"
#include "Path.h"
#include "Logger.h"


Application* Application::_pInstance = 0;


Application::Application() :
	_pConfig(new Configuration),
	_initialized(false),
	_pLogger(&Logger::get("ApplicationStartup"))
{
	setup();
}


Application::Application(int argc, char* argv[]) :
	_pConfig(new Configuration),
	_initialized(false),
	_pLogger(&Logger::get("ApplicationStartup"))
{
	setup();
	init(argc, argv);
}


Application::~Application()
{
	_pInstance = 0;
}


void Application::setup()
{
	ASSERT(_pInstance == 0);

	//_pConfig->add(new SystemConfiguration, PRIO_SYSTEM, false);
	//_pConfig->add(new MapConfiguration, PRIO_APPLICATION, true);

	//addSubsystem(new LoggingSubsystem);

	setUnixOptions(false);

	_pInstance = this;
}


void Application::addSubsystem(Subsystem* pSubsystem)
{
	CHECK_PTR(pSubsystem);

	_subsystems.push_back(pSubsystem);
}


void Application::init(int argc, char* argv[])
{
	setArgs(argc, argv);
	init();
}


void Application::init(const ArgVec& args)
{
	setArgs(args);
	init();
}


void Application::init()
{
	Path appPath;
	getApplicationPath(appPath);
	_pConfig->setString("application.path", appPath.toString());
	_pConfig->setString("application.name", appPath.getFileName());
	_pConfig->setString("application.baseName", appPath.getBaseName());
	_pConfig->setString("application.dir", appPath.parent().toString());
	_pConfig->setString("application.configDir", Path::configHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.cacheDir", Path::cacheHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.tempDir", Path::tempHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.dataDir", Path::dataHome() + appPath.getBaseName() + Path::separator());
	processOptions();
}


const char* Application::name() const
{
	return "Application";
}


void Application::initialize(Application& self)
{
	for (auto& pSub : _subsystems)
	{
		_pLogger->debug(std::string("Initializing subsystem: ") + pSub->name());
		pSub->initialize(self);
	}
	_initialized = true;
}


void Application::uninitialize()
{
	if (_initialized)
	{
		for (SubsystemVec::reverse_iterator it = _subsystems.rbegin(); it != _subsystems.rend(); ++it)
		{
			_pLogger->debug(std::string("Uninitializing subsystem: ") + (*it)->name());
			(*it)->uninitialize();
		}
		_initialized = false;
	}
}


void Application::reinitialize(Application& self)
{
	for (auto& pSub : _subsystems)
	{
		_pLogger->debug(std::string("Re-initializing subsystem: ") + pSub->name());
		pSub->reinitialize(self);
	}
}


void Application::setUnixOptions(bool flag)
{
	_options.setUnixOptions(flag);
}


int Application::loadConfiguration(int priority)
{
	int n = 0;
	Path appPath;
	getApplicationPath(appPath);
	return n;
}


void Application::loadConfiguration(const std::string& path, int priority)
{
}


std::string Application::commandName() const
{
	return _pConfig->getString("application.baseName");
}


std::string Application::commandPath() const
{
	return _pConfig->getString("application.path");
}


void Application::stopOptionsProcessing()
{
	_options.stopOptionsProcessing();
}


int Application::run()
{
	int rc = EXIT_CONFIG;

	try
	{
		initialize(*this);
		rc = EXIT_SOFTWARE;
		rc = main(_argv);
	}
	catch (Exception& exc)
	{
		logger().log(exc);
	}
	catch (std::exception& exc)
	{
		logger().error(exc.what());
	}
	catch (...)
	{
		logger().fatal("system exception");
	}

	uninitialize();
	return rc;
}


int Application::main(const ArgVec& args)
{
	return EXIT_OK;
}


void Application::setArgs(int argc, char* argv[])
{
	_command = argv[0];
	_pConfig->setInt("application.argc", argc);
	_argv.reserve(argc);
	std::string argvKey = "application.argv[";
	for (int i = 0; i < argc; ++i)
	{
		std::string arg(argv[i]);
		_pConfig->setString(argvKey + NumberFormatter::format(i) + "]", arg);
		_argv.push_back(arg);
	}
}


void Application::setArgs(const ArgVec& args)
{
	ASSERT(!args.empty());

	_command = args[0];
	_pConfig->setInt("application.argc", (int)args.size());
	_argv = args;
	std::string argvKey = "application.argv[";
	for (size_t i = 0; i < args.size(); ++i)
	{
		_pConfig->setString(argvKey + NumberFormatter::format(i) + "]", args[i]);
	}
}


void Application::processOptions()
{
	defineOptions(_options);

	_options.processArgs(_argv);
}


void Application::getApplicationPath(Path& appPath) const
{
	appPath = _command;
}


bool Application::findFile(Path& path) const
{
	if (path.isAbsolute()) return true;

	Path appPath;
	getApplicationPath(appPath);
	Path base = appPath.parent();
	do
	{
		Path p(base, path);
		File f(p);
		if (f.exists())
		{
			path = p;
			return true;
		}
		if (base.depth() > 0) base.popDirectory();
	} while (base.depth() > 0);
	return false;
}


bool Application::findAppConfigFile(const std::string& appName, const std::string& extension, Path& path) const
{
	ASSERT(!appName.empty());

	Path p(appName);
	p.setExtension(extension);
	bool found = findFile(p);
	if (!found)
	{
#if defined(_DEBUG)
		if (appName[appName.length() - 1] == 'd')
		{
			p.setBaseName(appName.substr(0, appName.length() - 1));
			found = findFile(p);
		}
#endif
	}
	if (found)
		path = p;
	return found;
}


bool Application::findAppConfigFile(const Path& basePath, const std::string& appName, const std::string& extension, Path& path) const
{
	ASSERT(!appName.empty());

	Path p(basePath, appName);
	p.setExtension(extension);
	bool found = findFile(p);
	if (!found)
	{
#if defined(_DEBUG)
		if (appName[appName.length() - 1] == 'd')
		{
			p.setBaseName(appName.substr(0, appName.length() - 1));
			found = findFile(p);
		}
#endif
	}
	if (found)
		path = p;
	return found;
}


void Application::defineOptions(OptionManager& options)
{
	for (auto& pSub : _subsystems)
	{
		pSub->defineOptions(options);
	}
}


void Application::handleOption(const std::string& name, const std::string& value)
{

}

