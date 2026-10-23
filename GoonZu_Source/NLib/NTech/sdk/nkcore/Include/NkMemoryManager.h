#pragma once

#include "NkAllocator.h"
#include "NkMemory.h"

class NkMemoryManager
{
public:
	static NkMemoryManager& Get();
	static bool IsInitialized();

	static void Init();
	static void Shutdown();

	void* Allocate(
		size_t stSize, 
		size_t stAlignment, 
		NkMemoryEventType eEventType,
		bool bProvideAccurateSizeOnDeallocate = false,
		const char *pcFile = NK_MEM_FILE_DEFAULT,
		int iLine = NK_MEM_LINE_DEFAULT,
		const char* pcFunction = NK_MEM_FUNCTION_DEFAULT);

	void Deallocate(
		void* pvMemory, 
		NkMemoryEventType eEventType,
		size_t stSizeinBytes= NK_MEM_DEALLOC_SIZE_DEFAULT);

	void* Reallocate(
		void* pvMemory, 
		size_t stSize, 
		size_t stAlignment, 
		NkMemoryEventType eEventType, 
		bool bProvideAccurateSizeOnDeallocate = false,
		size_t stSizeCurrent = NK_MEM_DEALLOC_SIZE_DEFAULT,
		const char *pcFile = NK_MEM_FILE_DEFAULT,
		int iLine = NK_MEM_LINE_DEFAULT,
		const char* pcFunction = NK_MEM_FUNCTION_DEFAULT);

	//static bool VerifyAddress(const void* pvMemory){};

	static const char* NkMemoryEventTypeToString(NkMemoryEventType eEventType);

protected:
	NkMemoryManager() {};
	NkMemoryManager(const NkMemoryManager&) {};

	static NkMemoryManager* ms_pkMemManager;

	NkAllocator* m_pkAllocator;
};

//---------------------------------------------------------------------------
inline NkMemoryManager& NkMemoryManager::Get()
{
	return *ms_pkMemManager;
}

//---------------------------------------------------------------------------
inline void* NkMemoryManager::Allocate(size_t stSizeInBytes, 
									   size_t stAlignment, 
									   NkMemoryEventType eEventType,
									   bool bProvideAccurateSizeOnDeallocate,
									   const char *pcFile,
									   int iLine, 
									   const char* pcFunction)
{
	NkAssert(m_pkAllocator);

	void* pvMemory = m_pkAllocator->Allocate(stSizeInBytes, stAlignment, 
		eEventType, bProvideAccurateSizeOnDeallocate, 
		pcFile, iLine, pcFunction);

	return pvMemory;
}

//---------------------------------------------------------------------------
inline void* NkMemoryManager::Reallocate(void* pvMemory, 
										 size_t stSizeInBytes, 
										 size_t stAlignment, 
										 NkMemoryEventType eEventType,
										 bool bProvideAccurateSizeOnDeallocate,
										 size_t stSizeCurrent,
										 const char *pcFile,
										 int iLine, 
										 const char* pcFunction)
{
	NkAssert(m_pkAllocator);

	void* pvNewMem = m_pkAllocator->Reallocate(pvMemory, stSizeInBytes,
		stAlignment, eEventType, bProvideAccurateSizeOnDeallocate,
		stSizeCurrent, pcFile, iLine, pcFunction);

	return pvNewMem;
}
//---------------------------------------------------------------------------
inline void NkMemoryManager::Deallocate(void* pvMemory, NkMemoryEventType eEventType, size_t stSizeInBytes)
{
	NkAssert(m_pkAllocator);

	m_pkAllocator->Deallocate(pvMemory, eEventType, stSizeInBytes);
}

//---------------------------------------------------------------------------
inline const char* NkMemoryManager::NkMemoryEventTypeToString(NkMemoryEventType eEventType)
{
	switch( eEventType )
	{
	case NK_UNKNOWN:
		return "NK_UNKNOWN";
	case NK_NEW:
		return "NK_NEW";
	case NK_NEW_ARRAY:
		return "NK_NEW_ARRAY";
	case NK_DELETE:
		return "NK_DELETE";
	case NK_DELETE_ARRAY:
		return "NK_DELETE_ARRAY";
	case NK_MALLOC:
		return "NK_MALLOC";
	case NK_REALLOC:
		return "NK_REALLOC";
	case NK_ALIGNEDMALLOC:
		return "NK_ALIGNEDMALLOC";
	case NK_ALIGNEDREALLOC:
		return "NK_ALIGNEDREALLOC";
	case NK_FREE:
		return "NK_FREE";
	case NK_ALIGNEDFREE:
		return "NK_ALIGNEDFREE";
	case NK_EXTERNALALLOC:
		return "NK_EXTERNALALLOC";
	case NK_EXTERNALFREE:
		return "NK_EXTERNALFREE";
	default:
		NkAssert(!"Unknown NkMemoryEventType  value!");
	};

	return 0;
}
