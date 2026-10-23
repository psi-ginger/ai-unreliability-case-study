/**
 *  @file
 *  @brief ���� ���
 *  @author mtorange
 *  @version $Revision: 1822 $
 *
 *  $Id: common.h 1822 2007-05-14 08:54:10Z kermi $
 *
 */

#ifndef COMMON_H_3C0C95CA624B35DB79D43591BD86E296
#define COMMON_H_3C0C95CA624B35DB79D43591BD86E296


#include <sys/types.h>


#ifndef __attribute__
	/* This feature is available in gcc versions 3.4 and later.  */
	#if (__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 4)) || __STRICT_ANSI__
		#define __attribute__(Spec) /* empty */
	#endif
#endif


#define __HAVE_GETNANOREV__ 1
namespace nano { 
	int GetRevision(); 
	void PrintBuildInfo();
}


#ifndef _WIN32
	#include "nanoconfig.h"
	#ifndef __NANO_OS_SOLARIS__
		#include <stdint.h>
	#else
		#include <inttypes.h>
	#endif
#else
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef _CRT_NONSTDC_NO_WARNINGS
	#define _CRT_NONSTDC_NO_WARNINGS
	#endif

	#ifndef _CRT_SECURE_NO_DEPRECATE
	#define _CRT_SECURE_NO_DEPRECATE
	#endif

	#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#endif

	#define __NANO_OS_WIN32__  		1
	#define INT32_MAX				INT_MAX
	#define INT32_MIN				INT_MIN
	#define INT64_MAX				LONG_MAX
	#define INT64_MIN				LONG_MIN
	typedef long					ssize_t;
	typedef long					suseconds_t;
	#ifndef _STDINT
	typedef __int8					int8_t;
	typedef __int16					int16_t;
	typedef __int32					int32_t;
	typedef __int64		 			int64_t;
	typedef unsigned __int8			uint8_t;
	typedef unsigned __int16		uint16_t;
	typedef unsigned __int32		uint32_t;
	typedef unsigned __int64		uint64_t;
	#endif
	typedef unsigned int			socklen_t;
	#define __attribute__(x)
	#define __NANO_CPU_32BIT__ 1
	#define __NANO_ENDIAN_LITTLE__ 1
	// ������ ��� �Ʒ��� ������ �������� ����� �����ؼ� 0���� ����
	#define _NANO_REV_ 0
	#define _NANO_BUILD_ 0

	#include <windows.h>
#endif /* _WIN32 */

#include <stdlib.h>

#if (__GNUC__ >= 3 && __GNUC_MINOR__ >= 4)
template <bool> struct __compile_time_assertion_failure_3E10BA06B262E296F23297A77FBE1BB0;
template <> struct __compile_time_assertion_failure_3E10BA06B262E296F23297A77FBE1BB0 <true> { enum {dummy = 1}; };
//! compile time assert.  �Ϲ� assert�� runtime�� check������ �̰��� compile time�� üũ�� �Ѵ�.
#define nano_ct_assert(x)  enum { dummy = sizeof(__compile_time_assertion_failure_3E10BA06B262E296F23297A77FBE1BB0< (bool)(x) >) }
#else
#define nano_ct_assert(x) /* empty */
#endif

#ifdef __NANO_ASSERT__
	#ifdef __USE_CUSTOM_ASSERT_FUNCTION__
		#include <stdio.h>
		#include <signal.h>
		void _nano_assert(bool value, const char *expression, const char *file, int line, const char *function);
		#define nano_assert(x) _nano_assert(x, #x, __FILE__, __LINE__, __PRETTY_FUNCTION__)
	#else
		#include <assert.h>
		#define nano_assert(x) assert(x)
	# endif
#else
	//! nano���� ����ϴ� assert �Լ�. configure�� ���� assert����� �Ѱ� ���� �Ҽ� �ִ�.
	#define nano_assert(x) ((void)0)
#endif /* __NANO_ASSERT__ */

#ifndef interface
//! interface type ����
//#define interface struct
#endif /* interface */

#ifdef __NANO_LEAK_CHECK__
#include "atomic.h"
#include "singleton.h"
#include <stdio.h>
#endif

#ifndef __NANO_MEM_DEBUG__
#include <string.h>
#endif /* __NANO_MEM_DEBUG__ */

// common types
namespace nano 
{
/**
	@addtogroup basictype
	@{
 */

#ifdef __NANO_MEM_DEBUG__
void *_nmalloc(size_t size, const char *file, int line);
void *_ncalloc(size_t nmemb, size_t size, const char *file, int line);
void _nfree(void *ptr, const char *file, int line);
void *_nrealloc(void *ptr, size_t size, const char *file, int line);
char *_nstrdup(const char *s, const char *file, int line);

#define nmalloc(size) _nmalloc(size, __FILE__, __LINE__)
#define ncalloc(nmemb, size) _ncalloc(nmemb, size, __FILE__, __LINE__)
#define nfree(ptr) _nfree(ptr, __FILE__, __LINE__)
#define nrealloc(ptr, size) _nrealloc(ptr, size, __FILE__, __LINE__)
#define nstrdup(s) _nstrdup(s, __FILE__, __LINE__)

#else /* turn off __NANO_MEM_DEBUG__ */

inline void *nmalloc(size_t size)
{
	return ::malloc(size);
}

inline void *ncalloc(size_t nmemb, size_t size)
{
	return ::calloc(nmemb, size);
}

inline void nfree(void *ptr)
{
	::free(ptr);
}

inline void *nrealloc(void *ptr, size_t size)
{
	return ::realloc(ptr, size);
}

inline char *nstrdup(const char *s)
{
#ifdef __NANO_OS_WIN32__
	return ::_strdup(s);
#else
	return ::strdup(s);
#endif
}

#endif /* __NANO_MEM_DEBUG __ */

#ifdef __NANO_LEAK_CHECK__
class AllocCounter: public AtomicCounter
{
	public:
	AllocCounter(int init=0):AtomicCounter(init) {}
	AllocCounter(const AllocCounter& rhs):AtomicCounter(rhs) {}
	~AllocCounter() { nano_assert(get() == 0); }
};
#endif

/**
 * @brief �⺻ �޸� �Ҵ� Ŭ����
 */
class DefaultAllocator
{
	public:
		void *malloc(size_t size) 
		{
#ifdef __NANO_LEAK_CHECK__
			(nano::SingletonHolder<nano::AllocCounter>::instance())++;
#endif
			return nmalloc(size); 
		}

		void *realloc(void *ptr,  size_t size)
		{
#ifdef __NANO_LEAK_CHECK__
			void *ret;
			ret = nrealloc(ptr, size); 
			if(size == 0)
				(nano::SingletonHolder<nano::AllocCounter>::instance())--;
			else if(ptr == NULL)
				(nano::SingletonHolder<nano::AllocCounter>::instance())++;
			return ret;
#else
			return nrealloc(ptr, size); 
#endif
		}

		void *calloc(size_t nmemb, size_t size)
		{
#ifdef __NANO_LEAK_CHECK__
			(nano::SingletonHolder<nano::AllocCounter>::instance())++;
#endif
			return ncalloc(nmemb, size);
		}

		void free(void *ptr)
		{
			nfree(ptr);
#ifdef __NANO_LEAK_CHECK__
			(nano::SingletonHolder<nano::AllocCounter>::instance())--;
#endif
		}
};

#if 0
/**
 * @brief �� �߿��� ���� ���� �����Ѵ�.
 */
template <typename T>
const T &min(const T &a, const T &b)
{
	return  (a < b) ? a : b;
}

/**
 * @brief �� �߿��� ū ���� �����Ѵ�.
 */
template <typename T>
const T &max(const T &a, const T &b)
{
	return  (a > b) ? a : b;
}
#endif

/**
 * @brief �ΰ��� ������ �̷������ ������ Ÿ��
 */
template <typename FIRST, typename SECOND>
struct Pair
{
	Pair(const FIRST &f, const SECOND &s) : first(f), second(s) {}
	Pair(const Pair &p) : first(p.first), second(p.second) {}
	Pair() {}

	FIRST first;
	SECOND second;
};

/**
 * @brief ũ�⸦ ���� ������ Ÿ��
 */
template <typename T>
struct BasicData
{
	BasicData(const T *f, size_t s) : ptr(f), size(s) {}
	BasicData(const BasicData &p) : ptr(p.ptr), size(p.size) {}
	BasicData() : ptr(NULL), size(0) {}

	const T &operator[](size_t idx) const 
	{ return ptr[idx]; }

	const T *ptr;
	size_t size;
};

/**
 * @brief �� ���� ���� �� ���
 * @param a �� ���
 * @param b �� ���
 * @return �� ���� ���� ��
 */
template<typename T>
const T &Min(const T &a, const T &b)
{
	return a<b?a:b;
}

/**
 * @brief �� ���� ���� ���� �� ���
 * @param a �� ���
 * @param b �� ���
 * @param c �� ���
 * @return �� ���� ���� ���� ��
 */
template<typename T>
const T &Min(const T &a, const T &b, const T &c)
{
	return Min(Min(a, b), c);
}

/**
 * @brief �� ���� ū �� ���
 * @param a �� ���
 * @param b �� ���
 * @return �� ���� ū ��
 */
template<typename T>
const T &Max(const T &a, const T &b)
{
	return a>b?a:b;
}

/**
 * @brief �� ���� ���� ū �� ���
 * @param a �� ���
 * @param b �� ���
 * @param c �� ���
 * @return �� ���� ���� ū ��
 */
template<typename T>
const T &Max(const T &a, const T &b, const T &c)
{
	return Max(Max(a, b), c);
}
/** @} */

}
#endif /* COMMON_H */
