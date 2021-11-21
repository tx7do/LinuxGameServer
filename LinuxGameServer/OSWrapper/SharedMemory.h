#pragma once

#include <string>

class SharedMemory
{
public:
	enum AccessMode
	{
		AM_READ = 0,
		AM_WRITE
	};

	SharedMemory();
	SharedMemory(const std::string& name, std::size_t size, SharedMemory::AccessMode mode, const void* addrHint = 0, bool server = true);
	SharedMemory(const SharedMemory& other);
	
	~SharedMemory();

	SharedMemory& operator = (const SharedMemory& other);

public:
	void swap(SharedMemory& other);

	char* begin() const;

	char* end() const;

protected:
	void map(const void* addrHint);

	void unmap();

	void close();

private:
	std::size_t _size;
	int         _fd;
	char* _address;
	SharedMemory::AccessMode _access;
	std::string _name;
	bool        _fileMapped;
	bool        _server;
};


inline char* SharedMemory::begin() const
{
	return _address;
}


inline char* SharedMemory::end() const
{
	return _address + _size;
}

inline void SharedMemory::swap(SharedMemory& other)
{
	*this = other;
}
