#pragma once

#include "NkAllocator.h"

class NKCORE_ENTRY NkStdAllocator : public NkAllocator
{
public:
	virtual void* Allocate(
		size_t& stSize,
		size_t& stAlignment,
		NkMemoryEventType eEventType,
		bool bProvideAccurateSizeOnDeallocate,
		const char* pcFile,
		int iLine,
		const char* pcFunction);

	virtual void Deallocate(
		void* pvMemory,
		NkMemoryEventType eEventType,
		size_t stSizeinBytes);

	virtual void* Reallocate(
		void* pvMemory,
		size_t& stSize,
		size_t& stAlignment,
		NkMemoryEventType eEventType,
		bool bProvideAccurateSizeOnDeallocate,
		size_t stSizeCurrent,
		const char* pcFile,
		int iLine,
		const char* pcFunction);

	virtual bool SetMarker(
		const char* pcMarkerType, 
		const char* pcClassifier,
		const char* pcString);

	virtual void Init();
	virtual void Shutdown();

	virtual bool VerifyAddress(const void* pvMemory);

protected:
	static size_t GetSizeFromAddress(void* pMemory);
	static void SetSizeToAddress(void* pMemory, size_t stSize);
};
