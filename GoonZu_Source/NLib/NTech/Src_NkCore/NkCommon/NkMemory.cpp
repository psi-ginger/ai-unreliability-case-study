#include "NkMemory.h"
#include "NkMemoryManager.h"

#ifdef NK_MEMORY_DEBUGGER

//---------------------------------------------------------------------------
// _NkMalloc (DEBUG)
//---------------------------------------------------------------------------
void* _NkMalloc(size_t stSizeInBytes, const char* pcSourceFile, int iSourceLine, const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());

	if( stSizeInBytes == 0 ) 
		stSizeInBytes = 1;

	void* pvMemory = NkMemoryManager::Get().Allocate(stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_MALLOC, false,
		pcSourceFile, iSourceLine, pcFunction);

	NkAssert(pvMemory != 0);

	return pvMemory;
}

//---------------------------------------------------------------------------
// _NkAlignedMalloc (DEBUG)
//---------------------------------------------------------------------------
void* _NkAlignedMalloc(size_t stSizeInBytes, size_t stAlignment,
					   const char* pcSourceFile, int iSourceLine, const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());
	
	if( stSizeInBytes == 0 )
		stSizeInBytes = 1;

	void* pvMemory = NkMemoryManager::Get().Allocate(stSizeInBytes, 
		stAlignment, NK_ALIGNEDMALLOC, false, 
		pcSourceFile, iSourceLine, pcFunction);

	return pvMemory;
}

//---------------------------------------------------------------------------
// _NkRealloc (DEBUG)
//---------------------------------------------------------------------------
void* _NkRealloc(void *pvMemory, 
				 size_t stSizeInBytes, 
				 const char* pcSourceFile,
				 int iSourceLine, 
				 const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());

	if( stSizeInBytes == 0 && pvMemory != 0 )
	{
		NkFree(pvMemory);
		return 0;
	}
	else if( pvMemory == 0 )
	{
		return _NkMalloc(stSizeInBytes, pcSourceFile, iSourceLine, pcFunction);
	}

	pvMemory = NkMemoryManager::Get().Reallocate(pvMemory, stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_REALLOC, false, 
		NK_MEM_DEALLOC_SIZE_DEFAULT,
		pcSourceFile, iSourceLine, pcFunction);

	return pvMemory;
}

//---------------------------------------------------------------------------
// _NkAlignedRealloc (DEBUG)
//---------------------------------------------------------------------------
void* _NkAlignedRealloc(void *pvMemory, 
						size_t stSizeInBytes, 
						size_t stAlignment, 
						const char* pcSourceFile, 
						int iSourceLine, 
						const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());

	if( stSizeInBytes == 0 && pvMemory != 0 )
	{
		NkAlignedFree(pvMemory);
		return 0;
	}
	else if( pvMemory == 0 )
	{
		return _NkAlignedMalloc(stSizeInBytes, stAlignment, 
			pcSourceFile, iSourceLine, pcFunction);
	}

	pvMemory = NkMemoryManager::Get().Reallocate(pvMemory, stSizeInBytes, 
		stAlignment, NK_ALIGNEDREALLOC, false, NK_MEM_DEALLOC_SIZE_DEFAULT, 
		pcSourceFile, iSourceLine, pcFunction);

	return pvMemory;
}

#else

//---------------------------------------------------------------------------
// _NkMalloc (RELEASE)
//---------------------------------------------------------------------------
void* _NkMalloc(size_t stSizeInBytes)
{
	if( stSizeInBytes == 0 )
		stSizeInBytes = 1;

	return NkMemoryManager::Get().Allocate(stSizeInBytes, NK_MEM_ALIGNMENT_DEFAULT, NK_MALLOC, false);
}

//---------------------------------------------------------------------------
// _NkAlignedMalloc (RELEASE)
//---------------------------------------------------------------------------
void* _NkAlignedMalloc(size_t stSizeInBytes, size_t stAlignment)
{
	if( stSizeInBytes == 0 )
		stSizeInBytes = 1;

	return NkMemoryManager::Get().Allocate(stSizeInBytes, stAlignment, NK_ALIGNEDMALLOC, false);
}

//---------------------------------------------------------------------------
// _NkRealloc (RELEASE)
//---------------------------------------------------------------------------
void* _NkRealloc(void *pvMemory, size_t stSizeInBytes)
{
	if( stSizeInBytes == 0 && pvMemory != 0 )
	{
		NkFree(pvMemory);
		return 0;
	}
	else if( pvMemory == 0 )
	{
		return _NkMalloc(stSizeInBytes);
	}

	return NkMemoryManager::Get().Reallocate(pvMemory, stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_REALLOC, false, NK_MEM_DEALLOC_SIZE_DEFAULT);
}

//---------------------------------------------------------------------------
// _NkAlignedRealloc (RELEASE)
//---------------------------------------------------------------------------
void* _NkAlignedRealloc(void *pvMemory, size_t stSizeInBytes, size_t stAlignment)
{
	if( stSizeInBytes == 0 && pvMemory != 0 )
	{
		NkAlignedFree(pvMemory);
		return 0;
	}
	else if( pvMemory == 0 )
	{
		return _NkAlignedMalloc(stSizeInBytes, stAlignment);
	}

	return NkMemoryManager::Get().Reallocate(pvMemory, stSizeInBytes, 
		stAlignment, NK_ALIGNEDREALLOC, false, NK_MEM_DEALLOC_SIZE_DEFAULT);
}

#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// NkVerifyAddress (DEBUG)
//---------------------------------------------------------------------------
/*
#ifdef NK_MEMORY_DEBUGGER
bool NkVerifyAddress( const void* pvMemory )
{
	if( pvMemory == 0 ) return true;

	NkAssert(NkMemoryManager::IsInitialized());
	return NkMemoryManager::Get().VerifyAddress(pvMemory);
}
#endif
//---------------------------------------------------------------------------
*/


//---------------------------------------------------------------------------
// _NkFree
//---------------------------------------------------------------------------
void _NkFree(void* pvMemory)
{
	if( pvMemory == 0 ) return;

	NkMemoryManager::Get().Deallocate(pvMemory, NK_FREE);
}

//---------------------------------------------------------------------------
// _NkAlignedFree
//---------------------------------------------------------------------------
void _NkAlignedFree(void* pvMemory)
{
	if( pvMemory == 0 ) return;

	NkMemoryManager::Get().Deallocate(pvMemory, NK_ALIGNEDFREE);
}



//---------------------------------------------------------------------------
// NkMemory
//---------------------------------------------------------------------------

#ifdef NK_MEMORY_DEBUGGER

void* NkMemory::operator new(size_t /*stSize*/)
{
	return 0;
}

void* NkMemory::operator new[](size_t /*stSize*/)
{
	return 0;
}

void* NkMemory::operator new(size_t stSizeInBytes, 
							 const char* pcSourceFile, 
							 int iSourceLine, 
							 const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());

	if( stSizeInBytes == 0 )
		stSizeInBytes = 1;

	void* pvMemory = NkMemoryManager::Get().Allocate(stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_NEW, true, 
		pcSourceFile, iSourceLine, pcFunction);

	return pvMemory;
}

void* NkMemory::operator new[](size_t stSizeInBytes, 
							   const char* pcSourceFile, 
							   int iSourceLine, 
							   const char* pcFunction)
{
	NkAssert(NkMemoryManager::IsInitialized());

	if( stSizeInBytes == 0 ) 
		stSizeInBytes = 1;

	void* pvMemory =  NkMemoryManager::Get().Allocate(stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_NEW_ARRAY, false,
		pcSourceFile, iSourceLine, pcFunction);        

	return pvMemory;
}
#else

void* NkMemory::operator new(size_t stSizeInBytes)
{
	if( stSizeInBytes == 0 )
		stSizeInBytes = 1;

	void* pvMemory = NkMemoryManager::Get().Allocate(stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_NEW, true);

	return pvMemory;
}
void* NkMemory::operator new[](size_t stSizeInBytes)
{
	if (stSizeInBytes == 0)
		stSizeInBytes = 1;

	void* pvMemory = NkMemoryManager::Get().Allocate(stSizeInBytes, 
		NK_MEM_ALIGNMENT_DEFAULT, NK_NEW_ARRAY, false);

	return pvMemory;
}
#endif
//---------------------------------------------------------------------------


void NkMemory::operator delete(void* pvMemory, size_t stElementSize)
{
	if( pvMemory )
	{
		NkAssert(NkMemoryManager::IsInitialized());
		NkMemoryManager::Get().Deallocate(pvMemory, NK_DELETE, stElementSize);
	}
}
//---------------------------------------------------------------------------
void NkMemory::operator delete[](void* pvMemory, size_t /*stElementSize*/)
{
	if( pvMemory )
	{
		NkAssert(NkMemoryManager::IsInitialized());       
		NkMemoryManager::Get().Deallocate(pvMemory, NK_DELETE_ARRAY);
	} 
}
//---------------------------------------------------------------------------
