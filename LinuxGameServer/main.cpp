#include <cstdio>
#include <iostream>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <condition_variable>

#include "Application.h"
#include "ServerApplication.h"
#include "DateTimeFormatter.h"

class SampleServer : public ServerApplication
{
public:
	SampleServer()
	{
	}

	~SampleServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration();
		ServerApplication::initialize(self);
		logger().information("starting up");
	}

	void uninitialize()
	{
		logger().information("shutting down");
		ServerApplication::uninitialize();
	}

	void displayHelp()
	{
	}

	int main(const ArgVec& args)
	{
		static std::atomic_bool state = { false };

		std::thread trd{ [] {
			Application& app = Application::instance();
			while (state.load())
			{
				sleep(5000);
				Application::instance().logger().information("busy doing nothing... " + DateTimeFormatter::format(app.uptime()));
			}
		} };

		waitForTerminationRequest();

		state = false;
		trd.join();
		
		return Application::EXIT_OK;
	}
};


GAME_SERVER_MAIN(SampleServer)
