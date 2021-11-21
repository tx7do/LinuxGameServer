#include "Error.h"
#include <string>
#include <string.h>
#include <errno.h>

int Error::last()
{
	return errno;
}


class StrErrorHelper
{
public:
	explicit StrErrorHelper(int err)
	{
		_buffer[0] = 0;

		setMessage(strerror(err));
	}

	~StrErrorHelper()
	{
	}

	const std::string& message() const
	{
		return _message;
	}

protected:
	void setMessage(int rc)
		/// Handles POSIX variant
	{
		_message = _buffer;
	}

	void setMessage(const char* msg)
		/// Handles GLIBC variant
	{
		_message = msg;
	}

private:
	char _buffer[256];
	std::string _message;
};

std::string Error::getMessage(int errorCode)
{
	StrErrorHelper helper(errorCode);
	return helper.message();
}
