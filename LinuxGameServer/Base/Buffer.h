/**********************************************************************
* Copyright (C) 2016 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Buffer.h
* 摘    要:		简单的内存缓冲
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once

#include <memory>
#include <cstring>
#include "Bugcheck.h"


template <class T>
class Buffer
{
public:
	Buffer(std::size_t size)
		: m_capacity(size)
		, m_size(0)
		, m_ptr(nullptr)
	{
		if (size > 0)
		{
			m_ptr = new T[size];
		}
	}

	~Buffer()
	{
		if (m_ptr)
		{
			delete[] m_ptr;
			m_ptr = nullptr;
		}
	}

	/// 改变当前Buffer的大小为newSize
	/// @param [in] newSize 新的缓存大小
	/// @param [in] preserveContent 是否保留已有数据标志
	void resize(std::size_t newSize, bool preserveContent = true)
	{
		ASSERT(newSize > 0 );
		if (newSize <= 0) return;

		// 如果已有的缓存已经够了,那就不必重新分配了.
		if (newSize <= m_capacity) return;

		T* ptr = new T[newSize];
		ASSERT(ptr);
		if (preserveContent)
		{
			std::size_t n = newSize > m_capacity ? m_capacity : newSize;
			if (m_ptr) std::memcpy(ptr, m_ptr, n);
		}
		if (m_ptr) delete [] m_ptr;
		m_ptr  = ptr;
		m_capacity = newSize;
	}

	/// 缓存可容纳的元素量
	std::size_t capacity() const
	{
		return m_capacity;
	}

	/// 缓存的数据大小
	std::size_t size() const
	{
		return m_size;
	}

	/// 缓存的起始指针
	T* begin()
	{
		return m_ptr;
	}
	const T* begin() const
	{
		return m_ptr;
	}

	/// 缓存的尾指针
	T* end()
	{
		if (m_ptr == 0) return 0;
		return m_ptr + m_size;
	}
	const T* end() const
	{
		if (m_ptr == 0) return 0;
		return m_ptr + m_size;
	}

	const T& operator [] (std::size_t index) const
	{
		ASSERT(index >= 0 && index < m_capacity);
		if (m_ptr == 0) return 0;

		return m_ptr[index];
	}
	T& operator [] (std::size_t index)
	{
		ASSERT(index >= 0 && index < m_capacity);
		if (m_ptr == 0) return 0;

		return m_ptr[index];
	}

	void setData(const T* pVals, std::size_t nSize)
	{
		ASSERT(nSize);
		if (nSize <= 0) return;
		if (nSize > capacity()) resize(nSize, false);
		std::memcpy(m_ptr, pVals, nSize);
		m_size = nSize;
	}

	void clear()
	{
		if (m_ptr)
		{
			std::memset(m_ptr, 0, m_capacity);
		}
		m_size = 0;
	}

private:
	Buffer();
	Buffer(const Buffer&);
	Buffer& operator = (const Buffer&);

	std::size_t	m_capacity;	///< 容积
	std::size_t	m_size;		///< 数据大小
	T*			m_ptr;		///< 数据指针
};
