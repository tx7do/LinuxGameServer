/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		SingletonHolder.h
* ժ    Ҫ:		
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once

#include <mutex>

template <class S>
class SingletonHolder
{
public:
	SingletonHolder()
	{
		m_pS = 0;
	}

	~SingletonHolder()
	{
		delete m_pS;
	}

	S* get()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (!m_pS) m_pS = new S;
		return m_pS;
	}

private:
	S*				m_pS;
	std::mutex		m_mutex;
};
