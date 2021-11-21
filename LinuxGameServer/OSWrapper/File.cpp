#include "File.h"
#include "Path.h"
#include "DirectoryIterator.h"
#include "Buffer.h"
#include "BaseException.h"
#include "Error.h"
#include "Bugcheck.h"

#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/statfs.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>
#include <cstring>
#include <thread>

#define STATFSFN statfs
#define STATFSSTRUCT statfs

File::File()
{
}


File::File(const std::string& path) : _path(path)
{
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}


File::File(const char* path) : File(std::string(path))
{
}


File::File(const Path& path) : File(path.toString())
{
}


File::File(const File& file) : File(file.getPath())
{
}


File::~File()
{
}


void File::setPath(const std::string& path)
{
	_path = path;
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}

File& File::operator = (const File& file)
{
	setPath(file.getPath());
	return *this;
}


File& File::operator = (const std::string& path)
{
	setPath(path);
	return *this;
}


File& File::operator = (const char* path)
{
	CHECK_PTR(path);
	setPath(path);
	return *this;
}


File& File::operator = (const Path& path)
{
	setPath(path.toString());
	return *this;
}


void File::swap(File& file)
{
	std::swap(_path, file._path);
}


bool File::exists() const
{
	ASSERT(!_path.empty());

	struct stat st;
	return stat(_path.c_str(), &st) == 0;
}


bool File::canRead() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IRUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IRGRP) != 0;
		else
			return (st.st_mode & S_IROTH) != 0 || geteuid() == 0;
	}
	else handleLastError(_path);
	return false;
}


bool File::canWrite() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IWUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IWGRP) != 0;
		else
			return (st.st_mode & S_IWOTH) != 0 || geteuid() == 0;
	}
	else handleLastError(_path);
	return false;
}


bool File::canExecute() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid() || geteuid() == 0)
			return (st.st_mode & S_IXUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IXGRP) != 0;
		else
			return (st.st_mode & S_IXOTH) != 0;
	}
	else handleLastError(_path);
	return false;
}


bool File::isFile() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISREG(st.st_mode);
	else
		handleLastError(_path);
	return false;
}


bool File::isDirectory() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISDIR(st.st_mode);
	else
		handleLastError(_path);
	return false;
}


bool File::isLink() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (lstat(_path.c_str(), &st) == 0)
		return S_ISLNK(st.st_mode);
	else
		handleLastError(_path);
	return false;
}


bool File::isDevice() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
	else
		handleLastError(_path);
	return false;
}


bool File::isHidden() const
{
	ASSERT(!_path.empty());
	Path p(_path);
	p.makeFile();

	return p.getFileName()[0] == '.';
}


Timestamp File::created() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_ctime);
	else
		handleLastError(_path);
	return 0;
}


Timestamp File::getLastModified() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_mtime);
	else
		handleLastError(_path);
	return 0;
}


File& File::setLastModified(const Timestamp& ts)
{
	ASSERT(!_path.empty());

	struct utimbuf tb;
	tb.actime = ts.epochTime();
	tb.modtime = ts.epochTime();
	if (utime(_path.c_str(), &tb) != 0)
		handleLastError(_path);

	return *this;
}


File::FileSize File::getSize() const
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return st.st_size;
	else
		handleLastError(_path);
	return 0;
}


File& File::setSize(FileSize size)
{
	ASSERT(!_path.empty());

	if (truncate(_path.c_str(), size) != 0)
		handleLastError(_path);

	return *this;
}


File& File::setWriteable(bool flag)
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0)
		handleLastError(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IWUSR;
	}
	else
	{
		mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0)
		handleLastError(_path);

	return *this;
}


File& File::setReadOnly(bool flag)
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0)
		handleLastError(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IWUSR;
	}
	else
	{
		mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0)
		handleLastError(_path);

	return *this;
}


File& File::setExecutable(bool flag)
{
	ASSERT(!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0)
		handleLastError(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IXUSR;
		if (st.st_mode & S_IRGRP)
			mode |= S_IXGRP;
		if (st.st_mode & S_IROTH)
			mode |= S_IXOTH;
	}
	else
	{
		mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0)
		handleLastError(_path);

	return *this;
}

void File::copyToImpl(const std::string& path, int options) const
{
	ASSERT(!_path.empty());

	int sd = open(_path.c_str(), O_RDONLY);
	if (sd == -1) handleLastError(_path);

	struct stat st;
	if (fstat(sd, &st) != 0)
	{
		int err = errno;
		close(sd);
		handleLastError(err, _path);
	}
	const long blockSize = st.st_blksize;
	int dd;
	if (options & OPT_FAIL_ON_OVERWRITE) {
		dd = open(path.c_str(), O_CREAT | O_TRUNC | O_EXCL | O_WRONLY, st.st_mode);
	}
	else {
		dd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	}
	if (dd == -1)
	{
		int err = errno;
		close(sd);
		handleLastError(err, path);
	}
	Buffer<char> buffer(blockSize);
	try
	{
		ssize_t n;
		while ((n = read(sd, buffer.begin(), blockSize)) > 0)
		{
			if (write(dd, buffer.begin(), n) != n)
				handleLastError(path);
		}
		if (n < 0)
		{
			handleLastError(_path);
		}
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0)
	{
		int err = errno;
		close(dd);
		handleLastError(err, path);
	}
	if (close(dd) != 0)
	{
		handleLastError(path);
	}
}

void File::copyTo(const std::string& path, int options) const
{
	Path src(getPath());
	Path dest(path);
	File destFile(path);
	if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
	{
		dest.makeDirectory();
		dest.setFileName(src.getFileName());
	}
	if (isDirectory())
		copyDirectory(dest.toString(), options);
	else
		copyToImpl(dest.toString(), options);
}


void File::copyDirectory(const std::string& path, int options) const
{
	File target(path);
	target.createDirectories();

	Path src(getPath());
	src.makeFile();
	DirectoryIterator it(src);
	DirectoryIterator end;
	for (; it != end; ++it)
	{
		it->copyTo(path, options);
	}
}


void File::moveTo(const std::string& path, int options)
{
	copyTo(path, options);
	remove(true);
	setPath(path);
}


void File::renameTo(const std::string& path, int options)
{
	ASSERT(!_path.empty());

	struct stat st;

	if (stat(path.c_str(), &st) == 0 && (options & OPT_FAIL_ON_OVERWRITE))
		throw FileExistsException(path, EEXIST);

	if (rename(_path.c_str(), path.c_str()) != 0)
		handleLastError(_path);

	setPath(path);
}


void File::linkTo(const std::string& path, LinkType type) const
{
	ASSERT(!_path.empty());

	if (type == 0)
	{
		if (link(_path.c_str(), path.c_str()) != 0)
			handleLastError(_path);
	}
	else
	{
		if (symlink(_path.c_str(), path.c_str()) != 0)
			handleLastError(_path);
	}
}

void File::removeImpl()
{
	ASSERT(!_path.empty());

	int rc;
	if (!isLink() && isDirectory())
		rc = rmdir(_path.c_str());
	else
		rc = unlink(_path.c_str());
	if (rc) handleLastError(_path);
}

void File::remove(bool recursive)
{
	if (recursive && !isLink() && isDirectory())
	{
		std::vector<File> files;
		list(files);
		for (auto& f : files)
		{
			f.remove(true);
		}

		int retry = 8;
		long sleep = 10;
		while (retry > 0)
		{
			try
			{
				removeImpl();
				retry = 0;
			}
			catch (DirectoryNotEmptyException&)
			{
				if (--retry == 0) throw;
				std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
				sleep *= 2;
			}
		}
	}
	else
	{
		removeImpl();
	}
}


bool File::createFile()
{
	ASSERT(!_path.empty());

	int n = open(_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		handleLastError(_path);
	return false;
}


bool File::createDirectory()
{
	ASSERT(!_path.empty());

	if (exists() && isDirectory())
		return false;
	if (mkdir(_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		handleLastError(_path);
	return true;
}


void File::createDirectories()
{
	if (!exists())
	{
		Path p(getPath());
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			File f(p);
			f.createDirectories();
		}
		try
		{
			createDirectory();
		}
		catch (FileExistsException&)
		{
		}
	}
}


void File::list(std::vector<std::string>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
}


File::FileSize File::totalSpace() const
{
	ASSERT(!_path.empty());

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastError(_path);

	return (FileSize)stats.f_blocks * (FileSize)stats.f_bsize;
}


File::FileSize File::usableSpace() const
{
	ASSERT(!_path.empty());

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastError(_path);

	return (FileSize)stats.f_bavail * (FileSize)stats.f_bsize;
}


File::FileSize File::freeSpace() const
{
	ASSERT(!_path.empty());

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastError(_path);

	return (FileSize)stats.f_bfree * (FileSize)stats.f_bsize;
}


void File::list(std::vector<File>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(*it);
		++it;
	}
}

void File::handleLastError(int err, const std::string& path)
{
	switch (err)
	{
	case EIO:
		throw IOException(path, err);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, err);
	case EACCES:
		throw FileAccessDeniedException(path, err);
	case ENOENT:
		throw FileNotFoundException(path, err);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, err);
	case EISDIR:
		throw OpenFileException("not a file", path, err);
	case EROFS:
		throw FileReadOnlyException(path, err);
	case EEXIST:
		throw FileExistsException(path, err);
	case ENOSPC:
		throw FileException("no space left on device", path, err);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, err);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw DirectoryNotEmptyException(path, err);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path, err);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, err);
	default:
		throw FileException(Error::getMessage(err), path, err);
	}
}

void File::handleLastError(const std::string& path)
{
	handleLastError(errno, path);
}
