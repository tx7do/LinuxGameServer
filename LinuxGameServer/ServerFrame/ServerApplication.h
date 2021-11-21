#pragma once

#include "Application.h"

class ServerApplication : public Application
{
public:
	ServerApplication();
	~ServerApplication();

	bool isInteractive() const;

	int run(int argc, char** argv);

	int run(const std::vector<std::string>& args);

	static void terminate();

protected:
	int run();
	void waitForTerminationRequest();
	void defineOptions(OptionManager& options);

private:
	void handleDaemon(const std::string& name, const std::string& value);
	void handleUMask(const std::string& name, const std::string& value);
	void handlePidFile(const std::string& name, const std::string& value);

	bool isDaemon(int argc, char** argv);
	void beDaemon();

};

#define GAME_SERVER_MAIN(App) \
	int main(int argc, char** argv)		\
	{									\
		try 							\
		{								\
			App app;					\
			return app.run(argc, argv);	\
		}								\
		catch (Exception& exc)	\
		{								\
			std::cerr << exc.displayText() << std::endl;	\
			return Application::EXIT_SOFTWARE; 	\
		}								\
	}
