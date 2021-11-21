
#include "ServerApplication.h"
#include "Process.h"
#include "BaseException.h"
#include "OptionManager.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fstream>

ServerApplication::ServerApplication()
{
}


ServerApplication::~ServerApplication()
{
}


bool ServerApplication::isInteractive() const
{
	bool runsInBackground = config().getBool("application.runAsDaemon", false) || config().getBool("application.runAsService", false);
	return !runsInBackground;
}


int ServerApplication::run()
{
	return Application::run();
}


void ServerApplication::terminate()
{
	Process::requestTermination(Process::id());
}


void ServerApplication::waitForTerminationRequest()
{
	sigset_t sset;
	::sigemptyset(&sset);
	if (!std::getenv("GS_ENABLE_DEBUGGER"))
	{
		::sigaddset(&sset, SIGINT);
	}
	::sigaddset(&sset, SIGQUIT);
	::sigaddset(&sset, SIGTERM);
	::sigprocmask(SIG_BLOCK, &sset, NULL);
	int sig;
	::sigwait(&sset, &sig);
}


int ServerApplication::run(int argc, char** argv)
{
	bool runAsDaemon = isDaemon(argc, argv);
	if (runAsDaemon)
	{
		beDaemon();
	}
	try
	{
		init(argc, argv);
		if (runAsDaemon)
		{
			int rc = ::chdir("/");
			if (rc != 0) return EXIT_OSERR;
		}
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


int ServerApplication::run(const std::vector<std::string>& args)
{
	bool runAsDaemon = false;
	for (const auto& arg : args)
	{
		if (arg == "--daemon")
		{
			runAsDaemon = true;
			break;
		}
	}
	if (runAsDaemon)
	{
		beDaemon();
	}
	try
	{
		init(args);
		if (runAsDaemon)
		{
			int rc = ::chdir("/");
			if (rc != 0) return EXIT_OSERR;
		}
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


bool ServerApplication::isDaemon(int argc, char** argv)
{
	std::string option("--daemon");
	for (int i = 1; i < argc; ++i)
	{
		if (option == argv[i])
			return true;
	}
	return false;
}


void ServerApplication::beDaemon()
{
	pid_t pid;
	if ((pid = ::fork()) < 0)
		throw SystemException("cannot fork daemon process");
	else if (pid != 0)
		::exit(0);

	::setsid();
	::umask(027);

	// attach stdin, stdout, stderr to /dev/null
	// instead of just closing them. This avoids
	// issues with third party/legacy code writing
	// stuff to stdout/stderr.
	FILE* fin = ::freopen("/dev/null", "r+", stdin);
	if (!fin) throw OpenFileException("Cannot attach stdin to /dev/null");
	FILE* fout = ::freopen("/dev/null", "r+", stdout);
	if (!fout) throw OpenFileException("Cannot attach stdout to /dev/null");
	FILE* ferr = ::freopen("/dev/null", "r+", stderr);
	if (!ferr) throw OpenFileException("Cannot attach stderr to /dev/null");
}


void ServerApplication::defineOptions(OptionManager& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("daemon", "", "Run application as a daemon.")
		.required(false)
		.repeatable(false)
		.callback(std::bind(&ServerApplication::handleDaemon, this, std::placeholders::_1, std::placeholders::_2)));

	options.addOption(
		Option("umask", "", "Set the daemon's umask (octal, e.g. 027).")
		.required(false)
		.repeatable(false)
		.argument("mask")
		.callback(std::bind(&ServerApplication::handleUMask, this, std::placeholders::_1, std::placeholders::_2)));

	options.addOption(
		Option("pidfile", "", "Write the process ID of the application to given file.")
		.required(false)
		.repeatable(false)
		.argument("path")
		.callback(std::bind(&ServerApplication::handlePidFile, this, std::placeholders::_1, std::placeholders::_2)));
}


void ServerApplication::handleDaemon(const std::string& name, const std::string& value)
{
	config().setBool("application.runAsDaemon", true);
}


void ServerApplication::handleUMask(const std::string& name, const std::string& value)
{
	int mask = 0;
	for (const auto ch : value)
	{
		mask *= 8;
		if (ch >= '0' && ch <= '7')
			mask += ch - '0';
		else
			throw InvalidArgumentException("umask contains non-octal characters", value);
	}
	umask(mask);
}


void ServerApplication::handlePidFile(const std::string& name, const std::string& value)
{
}
