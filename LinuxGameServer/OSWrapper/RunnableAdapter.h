/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		RunnableAdapter.h
* ժ    Ҫ:		�߳̽ӿڵ�������
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#ifndef __BaseModule_RunnableAdapter_H__
#define __BaseModule_RunnableAdapter_H__


#include "Runnable.h"


//////////////////////////////////////////////////////////////////////////
/// Runnable��������
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
