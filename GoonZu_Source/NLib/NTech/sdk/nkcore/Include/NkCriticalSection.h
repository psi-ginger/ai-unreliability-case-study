#ifndef _NKCRITICALSECTION_H
#define _NKCRITICALSECTION_H

class NKCORE_ENTRY NkCriticalSection
{
public:
	NkCriticalSection();
	~NkCriticalSection();

	void Lock();
	void Unlock();

protected:
	CRITICAL_SECTION m_kCS;
};

inline NkCriticalSection::NkCriticalSection()
{
	InitializeCriticalSection(&m_kCS);
}

inline NkCriticalSection::~NkCriticalSection()
{
	DeleteCriticalSection(&m_kCS);
}

inline void NkCriticalSection::Lock()
{
	EnterCriticalSection(&m_kCS);
}

inline void NkCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_kCS);
}


#endif