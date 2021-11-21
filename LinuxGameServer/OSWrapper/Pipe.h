#pragma once

#include <sys/types.h>

class Pipe
{
public:
	typedef int Handle; 

	enum CloseMode
	{
		CLOSE_READ = 0x01, 
		CLOSE_WRITE = 0x02, 
		CLOSE_BOTH = 0x03 
	};

	Pipe();
	Pipe(const Pipe& pipe);
	~Pipe();

	Pipe& operator = (const Pipe& pipe);

	ssize_t writeBytes(const void* buffer, int length);

	ssize_t readBytes(void* buffer, int length);

	Handle readHandle() const;

	Handle writeHandle() const;

	void close(CloseMode mode = CLOSE_BOTH);

protected:
	void closeRead();
	void closeWrite();

private:
	int _readfd;
	int _writefd;
};
