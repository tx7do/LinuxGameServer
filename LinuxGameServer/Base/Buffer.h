/**********************************************************************
* Copyright (C) 2016 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Buffer.h
* ժ    Ҫ:		�򵥵��ڴ滺��
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
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

	/// �ı䵱ǰBuffer�Ĵ�СΪnewSize
	/// @param [in] newSize �µĻ����С
	/// @param [in] preserveContent �Ƿ����������ݱ�־
	void resize(std::size_t newSize, bool preserveContent = true)
	{
		ASSERT(newSize > 0 );
		if (newSize <= 0) return;

		// ������еĻ����Ѿ�����,�ǾͲ������·�����.
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

	/// ��������ɵ�Ԫ����
	std::size_t capacity() const
	{
		return m_capacity;
	}

	/// ��������ݴ�С
	std::size_t size() const
	{
		return m_size;
	}

	/// �������ʼָ��
	T* begin()
	{
		return m_ptr;
	}
	const T* begin() const
	{
		return m_ptr;
	}

	/// �����βָ��
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

	std::size_t	m_capacity;	///< �ݻ�
	std::size_t	m_size;		///< ���ݴ�С
	T*			m_ptr;		///< ����ָ��
};
