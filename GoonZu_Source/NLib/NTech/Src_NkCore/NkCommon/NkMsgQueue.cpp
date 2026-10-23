//-------------------------------------------------------------------------------------------------
// NkMsgQueue.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "NkMsgQueue.h"

NkMsgQueue::NkMsgQueue(unsigned int uiSize)
{
	InitializeCriticalSection(&m_cs);

	m_pcBuffer = NkAlloc(char, uiSize);

	m_uiBufferSize = uiSize;
	m_uiFreeSize = uiSize;

	m_uiFrontPos = 0;
	m_uiBackPos = 0;

	m_uiMsgCount = 0;
}

NkMsgQueue::~NkMsgQueue()
{
	NkFree(m_pcBuffer);

	DeleteCriticalSection(&m_cs);
}

bool NkMsgQueue::PushMsg(NkMsg* pkMsg)
{
	if( pkMsg == NULL )
		return false;

	EnterCriticalSection(&m_cs);

	int iSize = pkMsg->m_usMsgSize;
	
	NkAssert(iSize >= sizeof(NkMsg));

	int iNeedSize = iSize + sizeof(NkMsg);
	int iFreeSize = m_uiBufferSize - m_uiBackPos;

	if( m_uiFreeSize < (unsigned int)iNeedSize )
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	if( iFreeSize < iNeedSize )
	{
		if( m_uiFreeSize - (unsigned int)iFreeSize < (unsigned int)iNeedSize )
		{
			LeaveCriticalSection(&m_cs);
			return false;
		}

		memset(&m_pcBuffer[ m_uiBackPos ], 0, sizeof(NkMsg));

		memcpy(m_pcBuffer, pkMsg, iSize);
		m_uiBackPos = iSize;
		
		InterlockedExchangeAdd((LPLONG)&m_uiFreeSize, -(iFreeSize + iSize));

		InterlockedIncrement((LPLONG)&m_uiMsgCount);

		LeaveCriticalSection(&m_cs);

		return true;
	}

	memcpy(&m_pcBuffer[ m_uiBackPos ], pkMsg, iSize);
	m_uiBackPos += iSize;

	InterlockedExchangeAdd((LPLONG)&m_uiFreeSize, -iSize);

	InterlockedIncrement((LPLONG)&m_uiMsgCount);

	LeaveCriticalSection(&m_cs);

	return true;
}

NkMsg* NkMsgQueue::GetFrontMsg()
{
	if( m_uiMsgCount == 0 )
		return NULL;

	NkMsg* pkMsg = (NkMsg*)&m_pcBuffer[ m_uiFrontPos ];

	if( pkMsg->m_usMsgSize == 0 )
		return (NkMsg*)m_pcBuffer;

	return pkMsg;
}

void NkMsgQueue::RemoveFrontMsg()
{
	if( m_uiMsgCount == 0 )
		return;

	NkMsg* pkMsg = (NkMsg*)&m_pcBuffer[ m_uiFrontPos ];

	unsigned short usMsgSize = pkMsg->m_usMsgSize;

	int iFreeSize = usMsgSize;

	if( usMsgSize == 0 )
	{
		pkMsg = (NkMsg*)m_pcBuffer;
		usMsgSize = pkMsg->m_usMsgSize;

		iFreeSize = (m_uiBufferSize - m_uiFrontPos) + usMsgSize;
		m_uiFrontPos = usMsgSize;
	}
	else
	{
		m_uiFrontPos += usMsgSize;
	}

	InterlockedExchangeAdd((LPLONG)&m_uiFreeSize, iFreeSize);

	InterlockedDecrement((LPLONG)&m_uiMsgCount);
}