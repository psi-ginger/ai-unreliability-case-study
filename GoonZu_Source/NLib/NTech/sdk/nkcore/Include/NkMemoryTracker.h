#pragma once

#include <windows.h>
#include "NkAllocator.h"

class NkConsoleOutput;

class NKCORE_ENTRY NkAllocUnit
{
public:
	size_t m_stAllocationID;
	DWORD m_dwAllocThreadId;
	float m_fAllocTime;

	void* m_pvMemory;
	NkMemoryEventType m_eAllocType;

	NkAllocUnit* m_pkNext;

	size_t m_stAlignment;
	size_t m_stSizeRequested;
	size_t m_stSizeAllocated;

	const char* m_pcFile;
	const char* m_pcFunc;
	unsigned int m_uiLine;

	void Reset()
	{
		m_stAllocationID = (size_t) - 1;
		m_stAlignment = 0;
		m_dwAllocThreadId = 0;
		m_pvMemory = NULL;
		m_stSizeRequested = 0;
		m_stSizeAllocated = 0;
		m_eAllocType = NK_UNKNOWN;
		m_pkNext = NULL;
		m_pcFile = NULL;
		m_pcFunc = NULL;
	}
};

class NKCORE_ENTRY NkMemoryTracker : public NkAllocator
{
public:
	NkMemoryTracker(NkAllocator* pkActualAllocator);
	~NkMemoryTracker();

	virtual void* Allocate(
		size_t& stSizeInBytes, 
		size_t& stAlignment,
		NkMemoryEventType eEventType, 
		bool bProvideAccurateSizeOnDeallocate,
		const char* pcFile, 
		int iLine,
		const char* pcFunction);

	virtual void Deallocate(
		void* pvMemory, 
		NkMemoryEventType eEventType, 
		size_t stSizeInBytes);

	virtual void* Reallocate(
		void* pvMemory, 
		size_t& stSizeInBytes,
		size_t& stAlignment, 
		NkMemoryEventType eEventType, 
		bool bProvideAccurateSizeOnDeallocate,
		size_t stSizeCurrent,
		const char* pcFile, 
		int iLine,
		const char* pcFunction);

	virtual bool SetMarker(const char* pcMarkerType, const char* pcClassifier,
		const char* pcString);

	virtual void Init();
	virtual void Shutdown();

	virtual bool VerifyAddress(const void* pvMemory);

	void ResetStats();

protected:
    static unsigned int AddressToHashIndex(const void* pvAddress);

	void OutputLeakedMemory();
	void OutputAllocUnit(NkAllocUnit* pkUnit);

	void InsertAllocUnit(NkAllocUnit* pkUnit);
	void RemoveAllocUnit(NkAllocUnit* pkUnit, NkAllocUnit* pkPrevUnit);

	NkAllocUnit* FindAllocUnit(const void* pkMemory, NkAllocUnit** ppkOutPrevUnit);

	void AllocReservedBlock();

	NkAllocUnit** m_ppkActiveMem;

	NkAllocUnit** m_ppkReservedBlock;
	unsigned int m_uiReservedBlockCount;

	NkAllocUnit* m_pkReservedFirst;
	NkAllocUnit* m_pkReservedLast;

	NkAllocator* m_pkActualAllocator;				// 실제 메모리 할당자

protected:
	size_t m_stActiveMemory;						// 현재 메모리 사용량
	size_t m_stPeakMemory;							// 순간 최대 메모리 사용량
	size_t m_stAccumulatedMemory;					// 누적 메모리 사용량

	size_t m_stActiveAllocationCount;				// 메모리 할당 횟수
	size_t m_stPeakAllocationCount;					// 순간 최대 메모리 할당 횟수
	size_t m_stAccumulatedAllocationCount;			// 누적 메모리 할당 횟수

	NkConsoleOutput* m_pkConsoleOutput;				// 콘솔 출력
};

inline unsigned int NkMemoryTracker::AddressToHashIndex(const void* pvAddress)
{
	return (unsigned int)(reinterpret_cast<size_t>(pvAddress) >> 4) & 65535;
}
