#pragma once

#include "BaseException.h"
#include <mutex>

class ErrorHandler
{
public:
	ErrorHandler();
	virtual ~ErrorHandler();

	virtual void exception(const Exception& exc);

	virtual void exception(const std::exception& exc);

	virtual void exception();

	static void handle(const Exception& exc);

	static void handle(const std::exception& exc);

	static void handle();

	static ErrorHandler* set(ErrorHandler* pHandler);

	static ErrorHandler* get();

protected:
	static ErrorHandler* defaultHandler();

private:
	static ErrorHandler* _pHandler;
	static std::mutex     _mutex;
};


//
// inlines
//
inline ErrorHandler* ErrorHandler::get()
{
	return _pHandler;
}
