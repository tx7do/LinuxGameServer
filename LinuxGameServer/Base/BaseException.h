/**********************************************************************
* Copyright (C) 2016 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		BaseException.h
* 摘    要:		异常处理工具
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once


#include <stdexcept>
#include <string>
#include <iostream>


class Exception : public std::exception
{
public:
	Exception(const std::string& msg, int code = 0);

	Exception(const std::string& msg, const std::string& arg, int code = 0);

	Exception(const std::string& msg, const Exception& nested, int code = 0);

	Exception(const Exception& exc);

	~Exception() throw();

	Exception& operator = (const Exception& exc);

	virtual const char* name() const throw();

	virtual const char* className() const throw();

	virtual const char* what() const throw();

	const Exception* nested() const;

	const std::string& message() const;

	int code() const;

	std::string displayText() const;

	virtual Exception* clone() const;

	virtual void rethrow() const;

protected:
	Exception(int code = 0);

private:
	std::string	m_msg;
	Exception*	m_pNested;
	int			m_code;
};

inline const Exception* Exception::nested() const
{
	return m_pNested;
}


inline const std::string& Exception::message() const
{
	return m_msg;
}


inline int Exception::code() const
{
	return m_code;
}




#define DECLARE_EXCEPTION(API, CLS, BASE)										\
class API CLS: public BASE														\
{																				\
public:																			\
	CLS(int code = 0);																\
	CLS(const std::string& msg, int code = 0);										\
	CLS(const std::string& msg, const std::string& arg, int code = 0);				\
	CLS(const std::string& msg, const Exception& exc, int code = 0);	\
	CLS(const CLS& exc);															\
	~CLS() throw();																	\
	CLS& operator = (const CLS& exc);												\
	const char* name() const throw();												\
	const char* className() const throw();											\
	Exception* clone() const;											\
	void rethrow() const;															\
};




#define IMPLEMENT_EXCEPTION(CLS, BASE, NAME)														\
	CLS::CLS(int code): BASE(code)																	\
{																								\
}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
{																								\
}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code)								\
	: BASE(msg, arg, code)																			\
{																								\
}																								\
	CLS::CLS(const std::string& msg, const Exception& exc, int code)					\
	: BASE(msg, exc, code)																			\
{																								\
}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
{																								\
}																								\
	CLS::~CLS() throw()																				\
{																								\
}																								\
	CLS& CLS::operator = (const CLS& exc)															\
{																								\
	BASE::operator = (exc);																			\
	return *this;																					\
}																								\
	const char* CLS::name() const throw()															\
{																								\
	return NAME;																					\
}																								\
	const char* CLS::className() const throw()														\
{																								\
	return typeid(*this).name();																	\
}																								\
	Exception* CLS::clone() const														\
{																								\
	return new CLS(*this);																			\
}																								\
	void CLS::rethrow() const																		\
{																								\
	throw *this;																					\
}

#define BaseModule_API

DECLARE_EXCEPTION(BaseModule_API, LogicException, Exception);
DECLARE_EXCEPTION(BaseModule_API, AssertionViolationException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, NullPointerException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, BugcheckException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, InvalidArgumentException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, NotImplementedException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, RangeException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, IllegalStateException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, InvalidAccessException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, SignalException, LogicException);
DECLARE_EXCEPTION(BaseModule_API, UnhandledException, LogicException);

DECLARE_EXCEPTION(BaseModule_API, RuntimeException, Exception);
DECLARE_EXCEPTION(BaseModule_API, NotFoundException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, ExistsException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, TimeoutException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, SystemException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, RegularExpressionException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, LibraryLoadException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, LibraryAlreadyLoadedException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, NoThreadAvailableException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, PropertyNotSupportedException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, PoolOverflowException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, NoPermissionException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, OutOfMemoryException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, DataException, RuntimeException);

DECLARE_EXCEPTION(BaseModule_API, DataFormatException, DataException);
DECLARE_EXCEPTION(BaseModule_API, SyntaxException, DataException);
DECLARE_EXCEPTION(BaseModule_API, CircularReferenceException, DataException);
DECLARE_EXCEPTION(BaseModule_API, PathSyntaxException, SyntaxException);
DECLARE_EXCEPTION(BaseModule_API, IOException, RuntimeException);
DECLARE_EXCEPTION(BaseModule_API, FileException, IOException)
DECLARE_EXCEPTION(BaseModule_API, FileExistsException, FileException)
DECLARE_EXCEPTION(BaseModule_API, FileNotFoundException, FileException)
DECLARE_EXCEPTION(BaseModule_API, PathNotFoundException, FileException)
DECLARE_EXCEPTION(BaseModule_API, FileReadOnlyException, FileException)
DECLARE_EXCEPTION(BaseModule_API, FileAccessDeniedException, FileException)
DECLARE_EXCEPTION(BaseModule_API, CreateFileException, FileException)
DECLARE_EXCEPTION(BaseModule_API, OpenFileException, FileException)
DECLARE_EXCEPTION(BaseModule_API, WriteFileException, FileException)
DECLARE_EXCEPTION(BaseModule_API, ReadFileException, FileException)
DECLARE_EXCEPTION(BaseModule_API, DirectoryNotEmptyException, FileException)

DECLARE_EXCEPTION(BaseModule_API, ApplicationException, Exception);
DECLARE_EXCEPTION(BaseModule_API, BadCastException, RuntimeException);

DECLARE_EXCEPTION(BaseModule_API, OptionException, DataException)
DECLARE_EXCEPTION(BaseModule_API, UnknownOptionException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, AmbiguousOptionException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, MissingOptionException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, MissingOptionArgumentException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, InvalidOptionArgumentException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, UnexpectedOptionArgumentException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, IncompatibleOptionsException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, DuplicateOptionException, OptionException)
DECLARE_EXCEPTION(BaseModule_API, EmptyOptionException, OptionException)
