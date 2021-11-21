#include "Pipe.h"
#include "BaseException.h"
#include "Bugcheck.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

Pipe::Pipe()
{
	int fds[2];
	int rc = pipe(fds);
	if (rc == 0)
	{
		_readfd = fds[0];
		_writefd = fds[1];
	}
	else throw CreateFileException("anonymous pipe");
}


Pipe::Pipe(const Pipe& pipe)
{
	*this = pipe;
}


Pipe::~Pipe()
{
	closeRead();
	closeWrite();
}


Pipe& Pipe::operator = (const Pipe& pipe)
{
	if (this != &pipe)
	{
		*this = pipe;
	}
	return *this;
}


void Pipe::close(CloseMode mode)
{
	switch (mode)
	{
	case CLOSE_READ:
		closeRead();
		break;
	case CLOSE_WRITE:
		closeWrite();
		break;
	default:
		closeRead();
		closeWrite();
		break;
	}
}

ssize_t Pipe::writeBytes(const void* buffer, int length)
{
	ASSERT(_writefd != -1);

	ssize_t n;
	do
	{
		n = ::write(_writefd, buffer, length);
	} while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw WriteFileException("anonymous pipe");
}


ssize_t Pipe::readBytes(void* buffer, int length)
{
	ASSERT(_readfd != -1);

	ssize_t n;
	do
	{
		n = ::read(_readfd, buffer, length);
	} while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw ReadFileException("anonymous pipe");
}


Pipe::Handle Pipe::readHandle() const
{
	return _readfd;
}


Pipe::Handle Pipe::writeHandle() const
{
	return _writefd;
}


void Pipe::closeRead()
{
	if (_readfd != -1)
	{
		::close(_readfd);
		_readfd = -1;
	}
}


void Pipe::closeWrite()
{
	if (_writefd != -1)
	{
		::close(_writefd);
		_writefd = -1;
	}
}
