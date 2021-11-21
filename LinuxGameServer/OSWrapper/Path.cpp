#include "Path.h"

#include "File.h"
#include "BaseException.h"
#include "StringTokenizer.h"
#include "Bugcheck.h"
#include "Ascii.h"

#include <algorithm>
#include <unistd.h>
#include <stdlib.h>
#include <climits>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <sys/types.h>
#include <pwd.h>


#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

static std::string getEnv(const std::string& name)
{
	const char* val = getenv(name.c_str());
	if (val)
		return std::string(val);
	else
		throw NotFoundException(name);
}


static bool hasEnv(const std::string& name)
{
	return getenv(name.c_str()) != 0;
}

Path::Path() : _absolute(false)
{
}


Path::Path(bool absolute) : _absolute(absolute)
{
}


Path::Path(const std::string& path)
{
	assign(path);
}


Path::Path(const std::string& path, Style style)
{
	assign(path, style);
}


Path::Path(const char* path)
{
	CHECK_PTR(path);
	assign(path);
}


Path::Path(const char* path, Style style)
{
	CHECK_PTR(path);
	assign(path, style);
}


Path::Path(const Path& path) :
	_node(path._node),
	_device(path._device),
	_name(path._name),
	_version(path._version),
	_dirs(path._dirs),
	_absolute(path._absolute)
{
}


Path::Path(Path&& path) noexcept :
	_node(std::move(path._node)),
	_device(std::move(path._device)),
	_name(std::move(path._name)),
	_version(std::move(path._version)),
	_dirs(std::move(path._dirs)),
	_absolute(std::move(path._absolute))
{
}


Path::Path(const Path& parent, const std::string& fileName) :
	_node(parent._node),
	_device(parent._device),
	_name(parent._name),
	_version(parent._version),
	_dirs(parent._dirs),
	_absolute(parent._absolute)
{
	makeDirectory();
	_name = fileName;
}


Path::Path(const Path& parent, const char* fileName) :
	_node(parent._node),
	_device(parent._device),
	_name(parent._name),
	_version(parent._version),
	_dirs(parent._dirs),
	_absolute(parent._absolute)
{
	makeDirectory();
	_name = fileName;
}


Path::Path(const Path& parent, const Path& relative) :
	_node(parent._node),
	_device(parent._device),
	_name(parent._name),
	_version(parent._version),
	_dirs(parent._dirs),
	_absolute(parent._absolute)
{
	resolve(relative);
}


Path::~Path()
{
}


Path& Path::operator = (const Path& path)
{
	return assign(path);
}


Path& Path::operator = (Path&& path) noexcept
{
	_node = std::move(path._node);
	_device = std::move(path._device);
	_name = std::move(path._name);
	_version = std::move(path._version);
	_dirs = std::move(path._dirs);
	_absolute = std::move(path._absolute);
	return *this;
}


Path& Path::operator = (const std::string& path)
{
	return assign(path);
}


Path& Path::operator = (const char* path)
{
	CHECK_PTR(path);
	return assign(path);
}


void Path::swap(Path& path)
{
	std::swap(_node, path._node);
	std::swap(_device, path._device);
	std::swap(_name, path._name);
	std::swap(_version, path._version);
	std::swap(_dirs, path._dirs);
	std::swap(_absolute, path._absolute);
}


Path& Path::assign(const Path& path)
{
	if (&path != this)
	{
		_node = path._node;
		_device = path._device;
		_name = path._name;
		_version = path._version;
		_dirs = path._dirs;
		_absolute = path._absolute;
	}
	return *this;
}


Path& Path::assign(const std::string& path)
{
	parseUnix(path);
	return *this;
}


Path& Path::assign(const std::string& path, Style style)
{
	switch (style)
	{
	case PATH_UNIX:
		parseUnix(path);
		break;
	case PATH_WINDOWS:
		parseWindows(path);
		break;
	case PATH_VMS:
		parseVMS(path);
		break;
	case PATH_NATIVE:
		assign(path);
		break;
	case PATH_GUESS:
		parseGuess(path);
		break;
	default:
		BUGCHECK();
	}
	return *this;
}


Path& Path::assign(const char* path)
{
	return assign(std::string(path));
}


std::string Path::toString() const
{
	return buildUnix();
}


std::string Path::toString(Style style) const
{
	switch (style)
	{
	case PATH_UNIX:
		return buildUnix();
	case PATH_WINDOWS:
		return buildWindows();
	case PATH_VMS:
		return buildVMS();
	case PATH_NATIVE:
	case PATH_GUESS:
		return toString();
	default:
		BUGCHECK();
	}
	return std::string();
}


bool Path::tryParse(const std::string& path)
{
	try
	{
		Path p;
		p.parse(path);
		assign(p);
		return true;
	}
	catch (...)
	{
		return false;
	}
}


bool Path::tryParse(const std::string& path, Style style)
{
	try
	{
		Path p;
		p.parse(path, style);
		assign(p);
		return true;
	}
	catch (...)
	{
		return false;
	}
}


Path& Path::parseDirectory(const std::string& path)
{
	assign(path);
	return makeDirectory();
}


Path& Path::parseDirectory(const std::string& path, Style style)
{
	assign(path, style);
	return makeDirectory();
}


Path& Path::makeDirectory()
{
	pushDirectory(_name);
	_name.clear();
	_version.clear();
	return *this;
}


Path& Path::makeFile()
{
	if (!_dirs.empty() && _name.empty())
	{
		_name = _dirs.back();
		_dirs.pop_back();
	}
	return *this;
}


Path& Path::makeAbsolute()
{
	return makeAbsolute(current());
}


Path& Path::makeAbsolute(const Path& base)
{
	if (!_absolute)
	{
		Path tmp = base;
		tmp.makeDirectory();
		for (const auto& d : _dirs)
		{
			tmp.pushDirectory(d);
		}
		_node = tmp._node;
		_device = tmp._device;
		_dirs = tmp._dirs;
		_absolute = base._absolute;
	}
	return *this;
}


Path Path::absolute() const
{
	Path result(*this);
	if (!result._absolute)
	{
		result.makeAbsolute();
	}
	return result;
}


Path Path::absolute(const Path& base) const
{
	Path result(*this);
	if (!result._absolute)
	{
		result.makeAbsolute(base);
	}
	return result;
}


Path Path::parent() const
{
	Path p(*this);
	return p.makeParent();
}


Path& Path::makeParent()
{
	if (_name.empty())
	{
		if (_dirs.empty())
		{
			if (!_absolute)
				_dirs.push_back("..");
		}
		else
		{
			if (_dirs.back() == "..")
				_dirs.push_back("..");
			else
				_dirs.pop_back();
		}
	}
	else
	{
		_name.clear();
		_version.clear();
	}
	return *this;
}


Path& Path::append(const Path& path)
{
	makeDirectory();
	_dirs.insert(_dirs.end(), path._dirs.begin(), path._dirs.end());
	_name = path._name;
	_version = path._version;
	return *this;
}


Path& Path::resolve(const Path& path)
{
	if (path.isAbsolute())
	{
		assign(path);
	}
	else
	{
		for (int i = 0; i < path.depth(); ++i)
			pushDirectory(path[i]);
		_name = path._name;
	}
	return *this;
}


Path& Path::setNode(const std::string& node)
{
	_node = node;
	_absolute = _absolute || !node.empty();
	return *this;
}


Path& Path::setDevice(const std::string& device)
{
	_device = device;
	_absolute = _absolute || !device.empty();
	return *this;
}


const std::string& Path::directory(size_t n) const
{
	ASSERT(0 <= n && n <= _dirs.size());

	if (n < _dirs.size())
		return _dirs[n];
	else
		return _name;
}


const std::string& Path::operator [] (size_t n) const
{
	ASSERT(0 <= n && n <= _dirs.size());

	if (n < _dirs.size())
		return _dirs[n];
	else
		return _name;
}


Path& Path::pushDirectory(const std::string& dir)
{
	if (!dir.empty() && dir != ".")
	{
		if (dir == "..")
		{
			if (!_dirs.empty() && _dirs.back() != "..")
				_dirs.pop_back();
			else if (!_absolute)
				_dirs.push_back(dir);
		}
		else _dirs.push_back(dir);
	}
	return *this;
}


Path& Path::popDirectory()
{
	ASSERT(!_dirs.empty());

	_dirs.pop_back();
	return *this;
}


Path& Path::popFrontDirectory()
{
	ASSERT(!_dirs.empty());

	StringVec::iterator it = _dirs.begin();
	_dirs.erase(it);
	return *this;
}


Path& Path::setFileName(const std::string& name)
{
	_name = name;
	return *this;
}


Path& Path::setBaseName(const std::string& name)
{
	std::string ext = getExtension();
	_name = name;
	if (!ext.empty())
	{
		_name.append(".");
		_name.append(ext);
	}
	return *this;
}


std::string Path::getBaseName() const
{
	std::string::size_type pos = _name.rfind('.');
	if (pos != std::string::npos)
		return _name.substr(0, pos);
	else
		return _name;
}


Path& Path::setExtension(const std::string& extension)
{
	_name = getBaseName();
	if (!extension.empty())
	{
		_name.append(".");
		_name.append(extension);
	}
	return *this;
}


std::string Path::getExtension() const
{
	std::string::size_type pos = _name.rfind('.');
	if (pos != std::string::npos)
		return _name.substr(pos + 1);
	else
		return std::string();
}


Path& Path::clear()
{
	_node.clear();
	_device.clear();
	_name.clear();
	_dirs.clear();
	_version.clear();
	_absolute = false;
	return *this;
}


std::string Path::current()
{
	std::string path;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)))
		path = cwd;
	else
		throw SystemException("cannot get current directory");
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
}


std::string Path::home()
{
	std::string path;
	if (hasEnv("HOME"))
	{
		path = getEnv("HOME");
	}
	else
	{
		struct passwd* pwd = getpwuid(getuid());
		if (pwd)
		{
			path = pwd->pw_dir;
		}
		else
		{
			pwd = getpwuid(geteuid());
			if (pwd)
				path = pwd->pw_dir;
			else
				path = "/";
		}
	}
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
}


std::string Path::configHome()
{
	std::string path;
	if (hasEnv("XDG_CONFIG_HOME"))
		path = getEnv("XDG_CONFIG_HOME");
	if (!path.empty())
		return path;

	path = home();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".config/");

	return path;
}


std::string Path::dataHome()
{
	std::string path;
	if (hasEnv("XDG_DATA_HOME"))
		path = getEnv("XDG_DATA_HOME");
	if (!path.empty())
		return path;

	path = home();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".local/share/");

	return path;
}


std::string Path::tempHome()
{
	std::string path = home();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append("Library/Caches/");

	return path;
}

std::string Path::cacheHome()
{
	std::string path;
	if (hasEnv("XDG_CACHE_HOME"))
		path = getEnv("XDG_CACHE_HOME");
	if (!path.empty())
		return path;

	path = home();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".cache/");

	return path;
}


std::string Path::temp()
{
	std::string path;
	char* tmp = getenv("TMPDIR");
	if (tmp)
	{
		path = tmp;
		std::string::size_type n = path.size();
		if (n > 0 && path[n - 1] != '/') path.append("/");
	}
	else
	{
		path = "/tmp/";
	}
	return path;
}


std::string Path::config()
{
	std::string path;
	path = "/etc/";
	return path;
}


std::string Path::null()
{
	return "/dev/null";
}


std::string Path::expand(const std::string& path)
{
	std::string result;
	std::string::const_iterator it = path.begin();
	std::string::const_iterator end = path.end();
	if (it != end && *it == '~')
	{
		++it;
		if (it != end && *it == '/')
		{
			const char* homeEnv = getenv("HOME");
			if (homeEnv)
			{
				result += homeEnv;
				std::string::size_type resultSize = result.size();
				if (resultSize > 0 && result[resultSize - 1] != '/')
					result.append("/");
			}
			else
			{
				result += home();
			}
			++it;
		}
		else result += '~';
	}
	while (it != end)
	{
		if (*it == '\\')
		{
			++it;
			if (*it == '$')
			{
				result += *it++;
			}
		}
		else if (*it == '$')
		{
			std::string var;
			++it;
			if (it != end && *it == '{')
			{
				++it;
				while (it != end && *it != '}') var += *it++;
				if (it != end) ++it;
			}
			else
			{
				while (it != end && (Ascii::isAlphaNumeric(*it) || *it == '_')) var += *it++;
			}
			char* val = getenv(var.c_str());
			if (val) result += val;
		}
		else result += *it++;
	}
	std::string::size_type found = result.find("//");
	while (found != std::string::npos)
	{
		result.replace(found, 2, "/");
		found = result.find("//", found + 1);
	}
	return result;
}


void Path::listRoots(std::vector<std::string>& roots)
{
	roots.clear();
	roots.push_back("/");
}


bool Path::find(StringVec::const_iterator it, StringVec::const_iterator end, const std::string& name, Path& path)
{
	while (it != end)
	{
		Path p(*it);
		p.makeDirectory();
		p.resolve(Path(name));
		File f(p);
		if (f.exists())
		{
			path = p;
			return true;
		}
		++it;
	}
	return false;
}


bool Path::find(const std::string& pathList, const std::string& name, Path& path)
{
	StringTokenizer st(pathList, std::string(1, pathSeparator()), StringTokenizer::TOK_IGNORE_EMPTY + StringTokenizer::TOK_TRIM);
	return find(st.begin(), st.end(), name, path);
}


void Path::parseUnix(const std::string& path)
{
	clear();

	std::string::const_iterator it = path.begin();
	std::string::const_iterator end = path.end();

	if (it != end)
	{
		if (*it == '/')
		{
			_absolute = true; ++it;
		}
		else if (*it == '~')
		{
			++it;
			if (it == end || *it == '/')
			{
				Path cwd(home());
				_dirs = cwd._dirs;
				_absolute = true;
			}
			else --it;
		}

		while (it != end)
		{
			std::string name;
			while (it != end && *it != '/') name += *it++;
			if (it != end)
			{
				if (_dirs.empty())
				{
					if (!name.empty() && *(name.rbegin()) == ':')
					{
						_absolute = true;
						_device.assign(name, 0, name.length() - 1);
					}
					else
					{
						pushDirectory(name);
					}
				}
				else pushDirectory(name);
			}
			else _name = name;
			if (it != end) ++it;
		}
	}
}


void Path::parseWindows(const std::string& path)
{
	clear();

	std::string::const_iterator it = path.begin();
	std::string::const_iterator end = path.end();

	if (it != end)
	{
		if (*it == '\\' || *it == '/') { _absolute = true; ++it; }
		if (_absolute && it != end && (*it == '\\' || *it == '/')) // UNC
		{
			++it;
			while (it != end && *it != '\\' && *it != '/') _node += *it++;
			if (it != end) ++it;
		}
		else if (it != end)
		{
			char d = *it++;
			if (it != end && *it == ':') // drive letter
			{
				if (_absolute || !((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z'))) throw PathSyntaxException(path);
				_absolute = true;
				_device += d;
				++it;
				if (it == end || (*it != '\\' && *it != '/')) throw PathSyntaxException(path);
				++it;
			}
			else --it;
		}
		while (it != end)
		{
			std::string name;
			while (it != end && *it != '\\' && *it != '/') name += *it++;
			if (it != end)
				pushDirectory(name);
			else
				_name = name;
			if (it != end) ++it;
		}
	}
	if (!_node.empty() && _dirs.empty() && !_name.empty())
		makeDirectory();
}


void Path::parseVMS(const std::string& path)
{
	clear();

	std::string::const_iterator it = path.begin();
	std::string::const_iterator end = path.end();

	if (it != end)
	{
		std::string name;
		while (it != end && *it != ':' && *it != '[' && *it != ';') name += *it++;
		if (it != end)
		{
			if (*it == ':')
			{
				++it;
				if (it != end && *it == ':')
				{
					_node = name;
					++it;
				}
				else _device = name;
				_absolute = true;
				name.clear();
			}
			if (it != end)
			{
				if (_device.empty() && *it != '[')
				{
					while (it != end && *it != ':' && *it != ';') name += *it++;
					if (it != end)
					{
						if (*it == ':')
						{
							_device = name;
							_absolute = true;
							name.clear();
							++it;
						}
					}
				}
			}
			if (name.empty())
			{
				if (it != end && *it == '[')
				{
					++it;
					if (it != end)
					{
						_absolute = true;
						if (*it == '.')
						{
							_absolute = false; ++it;
						}
						else if (*it == ']' || *it == '-')
							_absolute = false;
						while (it != end && *it != ']')
						{
							name.clear();
							if (*it == '-')
								name = "-";
							else
								while (it != end && *it != '.' && *it != ']') name += *it++;
							if (!name.empty())
							{
								if (name == "-")
								{
									if (_dirs.empty() || _dirs.back() == "..")
										_dirs.push_back("..");
									else
										_dirs.pop_back();
								}
								else _dirs.push_back(name);
							}
							if (it != end && *it != ']') ++it;
						}
						if (it == end) throw PathSyntaxException(path);
						++it;
						if (it != end && *it == '[')
						{
							if (!_absolute) throw PathSyntaxException(path);
							++it;
							if (it != end && *it == '.') throw PathSyntaxException(path);
							int d = int(_dirs.size());
							while (it != end && *it != ']')
							{
								name.clear();
								if (*it == '-')
									name = "-";
								else
									while (it != end && *it != '.' && *it != ']') name += *it++;
								if (!name.empty())
								{
									if (name == "-")
									{
										if (_dirs.size() > (size_t)d)
											_dirs.pop_back();
									}
									else _dirs.push_back(name);
								}
								if (it != end && *it != ']') ++it;
							}
							if (it == end) throw PathSyntaxException(path);
							++it;
						}
					}
					_name.clear();
				}
				while (it != end && *it != ';') _name += *it++;
			}
			else _name = name;
			if (it != end && *it == ';')
			{
				++it;
				while (it != end) _version += *it++;
			}
		}
		else _name = name;
	}
}


void Path::parseGuess(const std::string& path)
{
	bool hasBackslash = false;
	bool hasSlash = false;
	bool hasOpenBracket = false;
	bool hasClosBracket = false;
	bool isWindows = path.length() > 2 && path[1] == ':' && (path[2] == '/' || path[2] == '\\');
	std::string::const_iterator end = path.end();
	std::string::const_iterator semiIt = end;
	if (!isWindows)
	{
		for (std::string::const_iterator it = path.begin(); it != end; ++it)
		{
			switch (*it)
			{
			case '\\': hasBackslash = true; break;
			case '/':  hasSlash = true; break;
			case '[':  hasOpenBracket = true;
			case ']':  hasClosBracket = hasOpenBracket;
			case ';':  semiIt = it; break;
			}
		}
	}
	if (hasBackslash || isWindows)
	{
		parseWindows(path);
	}
	else if (hasSlash)
	{
		parseUnix(path);
	}
	else
	{
		bool isVMS = hasClosBracket;
		if (!isVMS && semiIt != end)
		{
			isVMS = true;
			++semiIt;
			while (semiIt != end)
			{
				if (*semiIt < '0' || *semiIt > '9')
				{
					isVMS = false; break;
				}
				++semiIt;
			}
		}
		if (isVMS)
			parseVMS(path);
		else
			parseUnix(path);
	}
}


std::string Path::buildUnix() const
{
	std::string result;
	if (!_device.empty())
	{
		result.append("/");
		result.append(_device);
		result.append(":/");
	}
	else if (_absolute)
	{
		result.append("/");
	}
	for (const auto& d : _dirs)
	{
		result.append(d);
		result.append("/");
	}
	result.append(_name);
	return result;
}


std::string Path::buildWindows() const
{
	std::string result;
	if (!_node.empty())
	{
		result.append("\\\\");
		result.append(_node);
		result.append("\\");
	}
	else if (!_device.empty())
	{
		result.append(_device);
		result.append(":\\");
	}
	else if (_absolute)
	{
		result.append("\\");
	}
	for (const auto& d : _dirs)
	{
		result.append(d);
		result.append("\\");
	}
	result.append(_name);
	return result;
}


std::string Path::buildVMS() const
{
	std::string result;
	if (!_node.empty())
	{
		result.append(_node);
		result.append("::");
	}
	if (!_device.empty())
	{
		result.append(_device);
		result.append(":");
	}
	if (!_dirs.empty())
	{
		result.append("[");
		if (!_absolute && _dirs[0] != "..")
			result.append(".");
		for (StringVec::const_iterator it = _dirs.begin(); it != _dirs.end(); ++it)
		{
			if (it != _dirs.begin() && *it != "..")
				result.append(".");
			if (*it == "..")
				result.append("-");
			else
				result.append(*it);
		}
		result.append("]");
	}
	result.append(_name);
	if (!_version.empty())
	{
		result.append(";");
		result.append(_version);
	}
	return result;
}


std::string Path::transcode(const std::string& path)
{
	return path;
}
