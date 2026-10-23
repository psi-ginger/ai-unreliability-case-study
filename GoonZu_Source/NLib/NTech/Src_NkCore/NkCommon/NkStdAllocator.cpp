#include "NkStdAllocator.h"

#define IS_POWER_OF_TWO(x)   (((x)&(x-1)) == 0)

void* NkStdAllocator::Allocate(size_t& stSizeInBytes,
							   size_t& stAlignment,
							   NkMemoryEventType /*eEventType*/,
							   bool /*bProvideAccurateSizeOnDeallocate*/,
							   const char* /*pcFile*/,
							   int /*iLine*/,
							   const char* /*pcFunction*/)
{
	NkAssert(IS_POWER_OF_TWO(stAlignment));

	return NkExtAlignedMalloc(stSizeInBytes, stAlignment);
}

//---------------------------------------------------------------------------
void NkStdAllocator::Deallocate(void* pvMemory, NkMemoryEventType /*eEventType*/, size_t /*stSizeInBytes*/)
{
	if( pvMemory == 0 )
		return;

	NkExtAlignedFree(pvMemory);
}

//---------------------------------------------------------------------------
void* NkStdAllocator::Reallocate(void* pvMemory,
								 size_t& stSizeInBytes,
								 size_t& stAlignment,
								 NkMemoryEventType /*eEventType*/,
								 bool /*bProvideAccurateSizeOnDeallocate*/,
								 size_t /*stSizeCurrent*/,
								 const char* /*pcFile*/,
								 int /*iLine*/,
								 const char* /*pcFunction*/)
{
	NkAssert(IS_POWER_OF_TWO(stAlignment));

	NkAssert(stSizeInBytes != 0);

	return NkExtAlignedRealloc(pvMemory, stSizeInBytes, stAlignment);
}

//---------------------------------------------------------------------------
bool NkStdAllocator::SetMarker(const char* /*pcMarkerType*/, const char* /*pcClassifier*/, const char* /*pcString*/)
{
	return true;
}

//---------------------------------------------------------------------------
void NkStdAllocator::Init()
{
}

//---------------------------------------------------------------------------
void NkStdAllocator::Shutdown()
{
}

//---------------------------------------------------------------------------
bool NkStdAllocator::VerifyAddress(const void* /*pvMemory*/)
{
	return true;
}

//---------------------------------------------------------------------------
size_t NkStdAllocator::GetSizeFromAddress(void* pMemory)
{
	size_t stSize;
	char* pcMemory = (char*) pMemory;
	char* pcSize = (char*)&stSize;
	NkAssert(sizeof(size_t) == 4);
	pcSize[0] = pcMemory[0];
	pcSize[1] = pcMemory[1];
	pcSize[2] = pcMemory[2];
	pcSize[3] = pcMemory[3];

	/*
	for (size_t i = 0; i < sizeof(size_t); i++)
	{
	pcSize[i] = pcMemory[i];
	}
	*/

	return stSize;
}

//---------------------------------------------------------------------------
void NkStdAllocator::SetSizeToAddress(void* pMemory, size_t stSize)
{
	char* pcMemory = (char*) pMemory;
	char* pcSize = (char*)&stSize;

	NkAssert(sizeof(size_t) == 4);
	pcMemory[0] = pcSize[0];
	pcMemory[1] = pcSize[1];
	pcMemory[2] = pcSize[2];
	pcMemory[3] = pcSize[3];

	/*
	for (size_t i = 0; i < sizeof(size_t); i++)
	{
	pcMemory[i] = pcSize[i];
	}
	*/
}
//---------------------------------------------------------------------------

