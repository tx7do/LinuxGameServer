#pragma once

#include <string>
#include <unistd.h>
#include <vector>
#include <map>

class Pipe;


class ProcessHandle
{
public:
	typedef pid_t PID;

	ProcessHandle(PID pid);
	ProcessHandle(const ProcessHandle& handle);
	~ProcessHandle();

	ProcessHandle& operator = (const ProcessHandle& handle);

	PID id() const;

	int wait() const;

	int tryWait() const;

private:
	ProcessHandle();

	friend class Process;

private:
	PID _pid;
};


class Process
{
public:
	typedef pid_t  PID;
	typedef std::vector<std::string> Args;
	typedef std::map<std::string, std::string>  Env;

	static PID id();

	static void times(long& userTime, long& kernelTime);

	static ProcessHandle launch(const std::string& command, const Args& args);

	static ProcessHandle launch(
		const std::string& command,
		const Args& args,
		const std::string& initialDirectory);

	static ProcessHandle launch(
		const std::string& command,
		const Args& args,
		Pipe* inPipe,
		Pipe* outPipe,
		Pipe* errPipe);

	static ProcessHandle launch(
		const std::string& command,
		const Args& args,
		const std::string& initialDirectory,
		Pipe* inPipe,
		Pipe* outPipe,
		Pipe* errPipe);

	static ProcessHandle launch(
		const std::string& command,
		const Args& args,
		Pipe* inPipe,
		Pipe* outPipe,
		Pipe* errPipe,
		const Env& env);

	static ProcessHandle launch(
		const std::string& command,
		const Args& args,
		const std::string& initialDirectory,
		Pipe* inPipe,
		Pipe* outPipe,
		Pipe* errPipe,
		const Env& env);

	static int wait(const ProcessHandle& handle);

	static int tryWait(const ProcessHandle& handle);

	static bool isRunning(const ProcessHandle& handle);

	static bool isRunning(PID pid);

	static void kill(ProcessHandle& handle);

	static void kill(PID pid);

	static void requestTermination(PID pid);

private:
	static ProcessHandle launchByForkExec(
		const std::string& command,
		const Args& args,
		const std::string& initialDirectory,
		Pipe* inPipe,
		Pipe* outPipe,
		Pipe* errPipe,
		const Env& env);
};

