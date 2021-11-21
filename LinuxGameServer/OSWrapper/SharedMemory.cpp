#include "SharedMemory.h"
#include "BaseException.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


/*
1. POSIX(更通用,需要-lrt)：
int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);
int ftruncate(int fd, off_t length);

2. SYSTEM V:
int shmget(key_t key, int size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
*/


SharedMemory::SharedMemory(const std::string& name, std::size_t size, SharedMemory::AccessMode mode, const void* addrHint, bool server) :
	_size(size),
	_fd(-1),
	_address(0),
	_access(mode),
	_name("/"),
	_fileMapped(false),
	_server(server)
{

}

SharedMemory::SharedMemory(const SharedMemory& other) 
{
	*this = other;
}

SharedMemory& SharedMemory::operator = (const SharedMemory& other)
{
	*this = other;
	return *this;
}

SharedMemory::~SharedMemory()
{
	unmap();
	close();
}


void SharedMemory::map(const void* addrHint)
{
	int access = PROT_READ;
	if (_access == SharedMemory::AM_WRITE)
		access |= PROT_WRITE;

	void* addr = ::mmap(const_cast<void*>(addrHint), _size, access, MAP_SHARED, _fd, 0);
	if (addr == MAP_FAILED)
		throw SystemException("Cannot map file into shared memory", _name);

	_address = static_cast<char*>(addr);
}


void SharedMemory::unmap()
{
	if (_address)
	{
		::munmap(_address, _size);
	}
}


void SharedMemory::close()
{
	if (_fd != -1)
	{
		::close(_fd);
		_fd = -1;
	}
	if (!_fileMapped && _server)
	{
		::shm_unlink(_name.c_str());
	}
}
