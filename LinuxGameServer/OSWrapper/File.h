#pragma once

#include "Timestamp.h"
#include "Types.h"
#include <vector>
#include <string>

class Path;


class File
{
public:
	typedef UInt64 FileSize;

	enum LinkType
	{
		LINK_HARD = 0,
		LINK_SYMBOLIC = 1
	};

	enum Options
	{
		OPT_FAIL_ON_OVERWRITE = 0x01
	};

	File();
	File(const std::string& path);
	File(const char* path);
	File(const Path& path);
	File(const File& file);

	virtual ~File();

	File& operator = (const File& file);
	File& operator = (const std::string& path);
	File& operator = (const char* path);
	File& operator = (const Path& path);
	void swap(File& file);
	const std::string& path() const;
	bool exists() const;
	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;

	bool isFile() const;

	bool isLink() const;

	bool isDirectory() const;

	bool isDevice() const;

	bool isHidden() const;

	Timestamp created() const;

	Timestamp getLastModified() const;

	File& setLastModified(const Timestamp& ts);

	FileSize getSize() const;

	File& setSize(FileSize size);

	File& setWriteable(bool flag = true);

	File& setReadOnly(bool flag = true);

	File& setExecutable(bool flag = true);

	void copyTo(const std::string& path, int options = 0) const;

	void moveTo(const std::string& path, int options = 0);

	void renameTo(const std::string& path, int options = 0);

	void linkTo(const std::string& path, LinkType type = LINK_SYMBOLIC) const;

	void remove(bool recursive = false);

	bool createFile();

	bool createDirectory();

	void createDirectories();

	void list(std::vector<std::string>& files) const;

	void list(std::vector<File>& files) const;

	FileSize totalSpace() const;

	FileSize usableSpace() const;

	FileSize freeSpace() const;

	bool operator == (const File& file) const;
	bool operator != (const File& file) const;
	bool operator <  (const File& file) const;
	bool operator <= (const File& file) const;
	bool operator >  (const File& file) const;
	bool operator >= (const File& file) const;

	static void handleLastError(const std::string& path);
	static void handleLastError(int err, const std::string& path);

protected:
	void copyDirectory(const std::string& path, int options = 0) const;

	const std::string& getPath() const;
	void setPath(const std::string& path);

	void copyToImpl(const std::string& path, int options) const;
	void removeImpl();

private:
	std::string _path;

	friend class DirectoryIterator;
	friend class LinuxDirectoryWatcherStrategy;
	friend class BSDDirectoryWatcherStrategy;
};


//
// inlines
//
inline const std::string& File::getPath() const
{
	return _path;
}


inline const std::string& File::path() const
{
	return getPath();
}


inline bool File::operator == (const File& file) const
{
	return getPath() == file.getPath();
}


inline bool File::operator != (const File& file) const
{
	return getPath() != file.getPath();
}


inline bool File::operator < (const File& file) const
{
	return getPath() < file.getPath();
}


inline bool File::operator <= (const File& file) const
{
	return getPath() <= file.getPath();
}


inline bool File::operator > (const File& file) const
{
	return getPath() > file.getPath();
}


inline bool File::operator >= (const File& file) const
{
	return getPath() >= file.getPath();
}


inline void swap(File& f1, File& f2)
{
	f1.swap(f2);
}

