/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		RunnableAdapter.h
* 摘    要:		线程接口的适配器
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#ifndef __BaseModule_RunnableAdapter_H__
#define __BaseModule_RunnableAdapter_H__


#include "Runnable.h"


//////////////////////////////////////////////////////////////////////////
/// Runnable的适配器
//////////////////////////////////////////////////////////////////////////
template <class C>
class RunnableAdapter : public Runnable
{
public:
	typedef void (C::*Callback)();

	RunnableAdapter(C& object, Callback method)
		: m_pObject(&object)
		, m_method(method)
	{
	}

	RunnableAdapter(const RunnableAdapter& ra)
		: m_pObject(ra.m_pObject)
		, m_method(ra.m_method)
	{
	}

	~RunnableAdapter()
	{
	}

	RunnableAdapter& operator = (const RunnableAdapter& ra)
	{
		m_pObject = ra.m_pObject;
		m_method  = ra.m_method;
		return *this;
	}

	void run()
	{
		(m_pObject->*m_method)();
	}

private:
	RunnableAdapter();

	C*			m_pObject;
	Callback	m_method;
};


#endif // __RunnableAdapter_H__
