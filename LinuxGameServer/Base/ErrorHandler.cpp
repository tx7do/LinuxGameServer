#include "ErrorHandler.h"
#include "SingletonHolder.h"
#include "Bugcheck.h"

ErrorHandler* ErrorHandler::_pHandler = ErrorHandler::defaultHandler();
std::mutex ErrorHandler::_mutex;


ErrorHandler::ErrorHandler()
{
}


ErrorHandler::~ErrorHandler()
{
}


void ErrorHandler::exception(const Exception& exc)
{
	DEBUGGER_MSG(exc.what());
}


void ErrorHandler::exception(const std::exception& exc)
{
	DEBUGGER_MSG(exc.what());
}


void ErrorHandler::exception()
{
	DEBUGGER_MSG("unknown exception");
}


void ErrorHandler::handle(const Exception& exc)
{
	std::unique_lock<std::mutex> lock(_mutex);
	try
	{
		_pHandler->exception(exc);
	}
	catch (...)
	{
	}
}


void ErrorHandler::handle(const std::exception& exc)
{
	std::unique_lock<std::mutex> lock(_mutex);
	try
	{
		_pHandler->exception(exc);
	}
	catch (...)
	{
	}
}


void ErrorHandler::handle()
{
	std::unique_lock<std::mutex> lock(_mutex);
	try
	{
		_pHandler->exception();
	}
	catch (...)
	{
	}
}


ErrorHandler* ErrorHandler::set(ErrorHandler* pHandler)
{
	CHECK_PTR(pHandler);

	std::unique_lock<std::mutex> lock(_mutex);
	ErrorHandler* pOld = _pHandler;
	_pHandler = pHandler;
	return pOld;
}


ErrorHandler* ErrorHandler::defaultHandler()
{
	static SingletonHolder<ErrorHandler> sh;
	return sh.get();
}
