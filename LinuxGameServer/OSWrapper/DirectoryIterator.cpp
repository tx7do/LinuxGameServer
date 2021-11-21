#include "DirectoryIterator.h"
#include "Path.h"

DirectoryIterator::DirectoryIterator()
{
}


DirectoryIterator::DirectoryIterator(const std::string& path) : _path(path), _pDir(0)
{
	Path p(path);
	p.makeFile();

	_pDir = opendir(p.toString().c_str());
	if (!_pDir) File::handleLastError(path);

	next();

	_path.makeDirectory();
	_path.setFileName(get());
	_file = _path;
}


DirectoryIterator::DirectoryIterator(const DirectoryIterator& iterator) : _path(iterator._path)
{
	_file = _path;
}


DirectoryIterator::DirectoryIterator(const File& file) : _path(file.path())
{
	_path.makeDirectory();
	_path.setFileName(get());
	_file = _path;
}


DirectoryIterator::DirectoryIterator(const Path& path) : _path(path)
{
	_path.makeDirectory();
	_path.setFileName(get());
	_file = _path;
}


DirectoryIterator::~DirectoryIterator()
{
	if (_pDir) ::closedir(_pDir);
}

const std::string& DirectoryIterator::next()
{
	do
	{
		struct dirent* pEntry = ::readdir(_pDir);
		if (pEntry)
			_current = pEntry->d_name;
		else
			_current.clear();
	} while (_current == "." || _current == "..");
	return _current;
}

DirectoryIterator& DirectoryIterator::operator = (const DirectoryIterator& it)
{
	if (&it != this)
	{
		_path = it._path;
		_file = _path;
	}
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const File& file)
{
	_path.parseDirectory(file.path());
	_path.setFileName(get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const Path& path)
{
	_path = path;
	_path.makeDirectory();
	_path.setFileName(get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const std::string& path)
{
	_path.parseDirectory(path);
	_path.setFileName(get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator ++ ()
{
	_path.setFileName(next());
	_file = _path;
	return *this;
}


DirectoryIterator DirectoryIterator::operator ++ (int dummy)
{
	_path.setFileName(next());
	_file = _path;
	return *this;
}
