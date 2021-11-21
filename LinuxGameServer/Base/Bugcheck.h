/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Bugcheck.h
* ժ    Ҫ:		�����⣬�ṩһЩ����ʱ������ĺꡣ
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once

#include <string>
#include "Platform.h"


class Bugcheck
{
public:
	/// ����ʧ�ܣ�����е�������������ģʽ�������׳�AssertionViolationException�쳣��
	static void assertion(const char* cond, const char* file, int line);

	/// ��ָ�룬����е�������������ģʽ�������׳�NullPointerException�쳣��
	static void nullPointer(const char* ptr, const char* file, int line);

	/// �ڲ���������е�������������ģʽ�������׳�BugcheckException�쳣��
	static void bugcheck(const char* file, int line);
	static void bugcheck(const char* msg, const char* file, int line);

	static void unexpected(const char* file, int line);

	/// �ڲ���������е�������������ģʽ��
	static void debugger(const char* file, int line);
	static void debugger(const char* msg, const char* file, int line);

protected:
	static std::string what(const char* msg, const char* file, int line);
};


#if defined(_DEBUG)
#define ASSERT_DBG(cond) \
	if (!(cond)) Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0
#else
#define ASSERT_DBG(cond)
#endif


#undef ASSERT
#if defined(_DEBUG)
#define ASSERT(cond) \
	if (!(cond)) Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0
#else
#define ASSERT(cond)
#endif


#define CHECK_PTR(ptr) \
	if (!(ptr)) Bugcheck::nullPointer(#ptr, __FILE__, __LINE__); else (void) 0


#define BUGCHECK() \
	Bugcheck::bugcheck(__FILE__, __LINE__)


#define BUGCHECK_MSG(msg) \
	Bugcheck::bugcheck(msg, __FILE__, __LINE__)

#define UNEXPECTED() \
	Bugcheck::unexpected(__FILE__, __LINE__);


#define DEBUGGER() \
	Bugcheck::debugger(__FILE__, __LINE__)


#define DEBUGGER_MSG(msg) \
	Bugcheck::debugger(msg, __FILE__, __LINE__)


#if defined(_DEBUG)
#	define STDOUT_DBG(outstr) \
	std::cout << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define STDOUT_DBG(outstr)
#endif


#if defined(_DEBUG)
#	define STDERR_DBG(outstr) \
	std::cerr << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define STDERR_DBG(outstr)
#endif



//////////////////////////////////////////////////////////////////////////
// ����ʱ����
//////////////////////////////////////////////////////////////////////////

template <bool x>
struct STATIC_ASSERTION_FAILURE;

template <> 
struct STATIC_ASSERTION_FAILURE<true> 
{
	enum 
	{ 
		value = 1 
	}; 
};

template <int x> 
struct static_assert_test
{
};

#if defined(__GNUC__) && (__GNUC__ == 3) && ((__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4))
#define STATIC_ASSERT(B) \
	typedef char JOIN(static_assert_typedef_, __LINE__) \
        [STATIC_ASSERTION_FAILURE<(bool) (B)>::value]
#else
#define STATIC_ASSERT(B) \
	typedef static_assert_test<sizeof(STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
		JOIN(static_assert_typedef_, __LINE__) UNUSED
#endif

