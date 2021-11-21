/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		SingletonHolder.h
* 摘    要:		
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
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
