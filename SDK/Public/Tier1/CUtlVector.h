#pragma once

#include "CUtlMemory.h"
#include <assert.h>     /* assert */
#define Assert assert
struct base_vector_t
{
public:

	enum
	{
		IsUtlVector = true
	};
};

template<class T, class A = CUtlMemory<T>> class CUtlVector : public base_vector_t
{
	typedef A CAllocator;

public:

	typedef T ElemType_t;
	typedef T* iterator;
	typedef const T* const_iterator;

	int Count() const;
	

	CAllocator m_Memory;

	int m_iSize;

	T* m_pElements;
};

template<typename T, class A> inline int CUtlVector<T, A>::Count() const
{
	return m_iSize;
}

