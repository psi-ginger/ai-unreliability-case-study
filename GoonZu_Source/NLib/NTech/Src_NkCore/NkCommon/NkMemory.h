#ifndef _NKMEMORY_H
#define _NKMEMORY_H

#include "NkCoreLibType.h"
#include "NkDebug.h"

#include <malloc.h>

#ifdef _DEBUG
#define NK_MEMORY_DEBUGGER
#endif


// 메모리 할당/해제 타입
enum NkMemoryEventType
{
	NK_UNKNOWN = 0,
	NK_NEW,
	NK_NEW_ARRAY,
	NK_DELETE,
	NK_DELETE_ARRAY,
	NK_MALLOC,
	NK_REALLOC,
	NK_ALIGNEDMALLOC,
	NK_ALIGNEDREALLOC,
	NK_FREE,
	NK_ALIGNEDFREE,
	NK_EXTERNALALLOC,
	NK_EXTERNALFREE
};

#define NK_MEM_FILE_DEFAULT 0
#define NK_MEM_LINE_DEFAULT -1
#define NK_MEM_FUNCTION_DEFAULT 0
#define NK_MEM_DEALLOC_SIZE_DEFAULT (size_t)-1
#define NK_MEM_MARKER_BEGIN "BEGIN"
#define NK_MEM_MARKER_END "END"

#define NK_MEM_ALIGNMENT_DEFAULT  4

NKCORE_ENTRY void _NkFree(void* pvMemory);
NKCORE_ENTRY void _NkAlignedFree(void* pvMemory);

// Build-dependent memory management routines
#if defined(NK_MEMORY_DEBUGGER)
NKCORE_ENTRY void* _NkMalloc(size_t stSizeInBytes, 
							 const char* pcSourceFile, int iSourceLine, const char* pcFunction);
NKCORE_ENTRY void* _NkAlignedMalloc(size_t stSizeInBytes, size_t stAlignment, 
									const char* pcSourceFile, int iSourceLine, const char* pcFunction);
NKCORE_ENTRY void* _NkRealloc(void *pvMemory, size_t stSizeInBytes, 
							  const char* pcSourceFile, int iSourceLine, const char* pcFunction); 
NKCORE_ENTRY void* _NkAlignedRealloc(void *pvMemory, size_t stSizeInBytes, size_t stAlignment,
									 const char* pcSourceFile, int iSourceLine, const char* pcFunction); 
//NKCORE_ENTRY bool NkVerifyAddress(const void* pvMemory);

#define NkNew new(__FILE__,__LINE__,__FUNCTION__)
#define NkDelete delete
#define NkAlloc(T, count) ((T*)_NkMalloc(sizeof(T)*(count), \
	__FILE__, __LINE__, __FUNCTION__)) 
#define NkAlignedAlloc(T, count, alignment) ((T*)_NkAlignedMalloc( sizeof(T)*(count), alignment, \
	__FILE__, __LINE__, __FUNCTION__)) 
#define NkAlignedMalloc(size, alignment) (_NkAlignedMalloc(size, alignment, \
	__FILE__, __LINE__, __FUNCTION__)) 
#define NkMalloc(size) (_NkMalloc(size, \
	__FILE__, __LINE__, __FUNCTION__)) 
#define NkRealloc(memblock, size) (_NkRealloc(memblock, size, \
	__FILE__, __LINE__, __FUNCTION__)) 
#define NkAlignedRealloc(memblock, size, alignment) (_NkAlignedRealloc( memblock, size, alignment, \
	__FILE__, __LINE__, __FUNCTION__))
#define NkFree(p) _NkFree(p)
#define NkAlignedFree(p) _NkAlignedFree(p)

#else

NKCORE_ENTRY void* _NkMalloc(size_t stSizeInBytes);
NKCORE_ENTRY void* _NkAlignedMalloc(size_t stSizeInBytes, size_t stAlignment);
NKCORE_ENTRY void* _NkRealloc(void *pvMemory, size_t stSizeInBytes); 
NKCORE_ENTRY void* _NkAlignedRealloc(void *pvMemory, size_t stSizeInBytes, size_t stAlignment); 

#define NkVerifyAddress(x) false

#define NkNew new
#define NkDelete delete
#define NkAlloc(T, count) ((T*)_NkMalloc(sizeof(T)*(count)))
#define NkAlignedAlloc(T, count, alignment) ((T*)_NkAlignedMalloc(sizeof(T)*(count), alignment))
#define NkAlignedMalloc(size, alignment) (_NkAlignedMalloc(size,alignment)) 
#define NkMalloc(size) (_NkMalloc(size)) 
#define NkRealloc(memblock, size) (_NkRealloc(memblock, size))
#define NkAlignedRealloc(memblock, size, alignment) (_NkAlignedRealloc(memblock, size, alignment))
#define NkFree(p) _NkFree(p)
#define NkAlignedFree(p) _NkAlignedFree(p)

#endif // NK_MEMORY_DEBUGGER

#define NkExtNew new
#define NkExtDelete delete
#define NkExtAlloc(T, count) ((T*)malloc(sizeof(T)*(count)))
#define NkExtMalloc malloc
#define NkExtRealloc realloc
#define NkExtFree free
#define NkExtCalloc calloc
#define NkExtAlignedMalloc _aligned_malloc
#define NkExtAlignedRealloc _aligned_realloc
#define NkExtAlignedFree _aligned_free
#define NkAlignOf(T) __alignof(T)
#define NkStackAlloc(T, count) (T*)_malloca(sizeof(T)*(count))
#define NkStackFree _freea

class NkMemory
{
#ifdef NK_MEMORY_DEBUGGER
private:
	static void* operator new(size_t stSize);
	static void* operator new[](size_t stSize);

public:
	static void *operator new(
		size_t stSize, 
		const char *pcSourceFile, 
		int iSourceLine, 
		const char* pcFunction);

	static void *operator new[](
		size_t stSize, 
		const char *pcSourceFile, 
		int iSourceLine, 
		const char* pcFunction);

	static void operator delete(
		void* /*pvMemory*/, 
		const char* /*pcSourceFile*/, 
		int /*iSourceLine*/, 
		const char* /*pcFunction*/){}

	static void operator delete[](
		void* /*pvMemory*/, 
		const char* /*pcSourceFile*/, 
		int /*iSourceLine*/, 
		const char* /*pcFunction*/){}
#else
public:
	static void* operator new(size_t stSize);
	static void* operator new[](size_t stSize);
#endif

public:
	static void operator delete(void *pvMemory, size_t stElementSize);
	static void operator delete[](void *pvMemory, size_t stElementSize);

	static void* operator new( size_t /*stSize*/, void* p ) { return p; }
	static void* operator new[]( size_t /*stSize*/, void* p ) { return p; }

	static void operator delete( void *, void* ) {}
	static void operator delete[]( void *, void* ) {}
};

#endif
