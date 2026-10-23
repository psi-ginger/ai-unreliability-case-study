#pragma once

#include "NkMemory.h"

class NKCORE_ENTRY NkAllocator
{
public:
	virtual ~NkAllocator(){};

	virtual void* Allocate(
		size_t& stSizeInBytes, 
		size_t& stAlignment,
		NkMemoryEventType eEventType, 
		bool bProvideAccurateSizeOnDeallocate,
		const char* pcFile, 
		int iLine,
		const char* pcFunction) = 0;

	virtual void Deallocate(
		void* pvMemory, 
		NkMemoryEventType eEventType, 
		size_t stSizeInBytes) = 0;

	virtual void* Reallocate(
		void* pvMemory, 
		size_t& stSizeInBytes,
		size_t& stAlignment, 
		NkMemoryEventType eEventType, 
		bool bProvideAccurateSizeOnDeallocate,
		size_t stSizeCurrent,
		const char* pcFile, 
		int iLine,
		const char* pcFunction) = 0;

	virtual bool SetMarker(const char* pcMarkerType, const char* pcClassifier,
		const char* pcString) = 0;

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual bool VerifyAddress(const void* pvMemory) = 0;
};
