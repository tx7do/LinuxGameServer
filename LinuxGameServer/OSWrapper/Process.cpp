#include "Process.h"

#include "BaseException.h"
#include "NumberFormatter.h"
#include "Bugcheck.h"
#include "Pipe.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <vector>


namespace
{
	std::vector<char> getEnvironmentVariablesBuffer(const Process::Env& env)
	{
		std::vector<char> envbuf;
		std::size_t pos = 0;

		for (const auto& p : env)
		{
			std::size_t envlen = p.first.length() + p.second.length() + 1;

			envbuf.resize(pos + envlen + 1);
			std::copy(p.first.begin(), p.first.end(), &envbuf[pos]);
			pos += p.first.length();
			envbuf[pos] = '=';
			++pos;
			std::copy(p.second.begin(), p.second.end(), &envbuf[pos]);
			pos += p.second.length();

			envbuf[pos] = '\0';
			++pos;
		}

		envbuf.resize(pos + 1);
		envbuf[pos] = '\0';

		return envbuf;
	}
}

//
// ProcessHandle
//

ProcessHandle::ProcessHandle(PID pid) : _pid(pid)
{
}

ProcessHandle::ProcessHandle(const ProcessHandle& handle) : _pid(handle._pid)
{
}


ProcessHandle::~ProcessHandle()
{
}


ProcessHandle& ProcessHandle::operator = (const ProcessHandle& handle)
{
	if (&handle != this)
	{
		this->_pid = handle._pid;
	}
	return *this;
}


ProcessHandle::PID ProcessHandle::id() const
{
	return _pid;
}


int ProcessHandle::wait() const
{
	int status = 0;
	int rc = 0;
	do
	{
		rc = waitpid(_pid, &status, 0);
	} while (rc < 0 && errno == EINTR);
	if (rc != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));

	if (WIFEXITED(status)) // normal termination
		return WEXITSTATUS(status);
	else // termination by a signal
		return 256 + WTERMSIG(status);
}


int ProcessHandle::tryWait() const
{
	int status = 0;
	int rc = 0;
	do
	{
		rc = waitpid(_pid, &status, WNOHANG);
	} while (rc < 0 && errno == EINTR);
	if (rc == 0)
		return -1;
	if (rc != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));
	if (WIFEXITED(status)) // normal termination
		return WEXITSTATUS(status);
	else // termination by a signal
		return 256 + WTERMSIG(status);
}


//
// Process
//
ProcessHandle Process::launch(const std::string& command, const Args& args)
{
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launch(command, args, initialDirectory, 0, 0, 0, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory)
{
	Env env;
	return ProcessHandle(launch(command, args, initialDirectory, 0, 0, 0, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe)
{
	ASSERT(inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launch(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe)
{
	ASSERT(inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	Env env;
	return ProcessHandle(launch(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env)
{
	ASSERT(inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	return launchByForkExec(command, args, initialDirectory, inPipe, outPipe, errPipe, env);
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env)
{
	ASSERT(inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	return launchByForkExec(command, args, initialDirectory, inPipe, outPipe, errPipe, env);
}


int Process::wait(const ProcessHandle& handle)
{
	return handle.wait();
}


int Process::tryWait(const ProcessHandle& handle)
{
	return handle.tryWait();
}


void Process::kill(ProcessHandle& handle)
{
	kill(handle.id());
}


void Process::kill(PID pid)
{
	if (::kill(pid, SIGKILL) != 0)
	{
		switch (errno)
		{
		case ESRCH:
			throw NotFoundException("cannot kill process");
		case EPERM:
			throw NoPermissionException("cannot kill process");
		default:
			throw SystemException("cannot kill process");
		}
	}
}

bool Process::isRunning(const ProcessHandle& handle)
{
	return isRunning(handle.id());
}


bool Process::isRunning(PID pid)
{
	if (::kill(pid, 0) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Process::requestTermination(PID pid)
{
	if (::kill(pid, SIGINT) != 0)
	{
		switch (errno)
		{
		case ESRCH:
			throw NotFoundException("cannot terminate process");
		case EPERM:
			throw NoPermissionException("cannot terminate process");
		default:
			throw SystemException("cannot terminate process");
		}
	}
}

ProcessHandle Process::launchByForkExec(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env)
{
	// On some systems, sysconf(_SC_OPEN_MAX) returns a ridiculously high number,
	// which would closing all file descriptors up to that number extremely slow.
	// We therefore limit the maximum number of file descriptors we close.
	const long CLOSE_FD_MAX = 100000;

	// We must not allocated memory after fork(),
	// therefore allocate all required buffers first.
	std::vector<char> envChars = getEnvironmentVariablesBuffer(env);
	std::vector<char*> argv(args.size() + 2);
	int i = 0;
	argv[i++] = const_cast<char*>(command.c_str());
	for (const auto& a : args)
	{
		argv[i++] = const_cast<char*>(a.c_str());
	}
	argv[i] = nullptr;

	const char* pInitialDirectory = initialDirectory.empty() ? 0 : initialDirectory.c_str();

	int pid = ::fork();
	if (pid < 0)
	{
		throw SystemException("Cannot fork process for", command);
	}
	else if (pid == 0)
	{
		if (pInitialDirectory)
		{
			if (chdir(pInitialDirectory) != 0)
			{
				_exit(72);
			}
		}

		// set environment variables
		char* p = &envChars[0];
		while (*p)
		{
			putenv(p);
			while (*p) ++p;
			++p;
		}

		// setup redirection
		if (inPipe)
		{
			dup2(inPipe->readHandle(), STDIN_FILENO);
			inPipe->close(Pipe::CLOSE_BOTH);
		}
		// outPipe and errPipe may be the same, so we dup first and close later
		if (outPipe) dup2(outPipe->writeHandle(), STDOUT_FILENO);
		if (errPipe) dup2(errPipe->writeHandle(), STDERR_FILENO);
		if (outPipe) outPipe->close(Pipe::CLOSE_BOTH);
		if (errPipe) errPipe->close(Pipe::CLOSE_BOTH);
		// close all open file descriptors other than stdin, stdout, stderr
		long fdMax = sysconf(_SC_OPEN_MAX);
		// on some systems, sysconf(_SC_OPEN_MAX) returns a ridiculously high number
		if (fdMax > CLOSE_FD_MAX) fdMax = CLOSE_FD_MAX;
		for (long i = 3; i < fdMax; ++i)
		{
			::close((int)i);
		}

		execvp(argv[0], &argv[0]);
		_exit(72);
	}

	if (inPipe)  inPipe->close(Pipe::CLOSE_READ);
	if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
	if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
	return ProcessHandle(pid);
}

Process::PID Process::id()
{
	return getpid();
}


void Process::times(long& userTime, long& kernelTime)
{
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	userTime = usage.ru_utime.tv_sec;
	kernelTime = usage.ru_stime.tv_sec;
}
