#ifndef _AUTHTHREAD_H_
#define _AUTHTHREAD_H_

#include <Windows.h>
#include "AuthInfo.h"

#define MAX_AUTHINFO_QUEUE_SIZE		500

class AuthInfoQueue
{
public:
	AuthInfoQueue()
	{
		ZeroMemory(m_AuthInfos, sizeof(m_AuthInfos));
		m_Head	= 0;
		m_Tail	= 0;
	}

	~AuthInfoQueue()
	{

	}

	BOOL QueueIn(AuthInfo Info)
	{
		if( (m_Tail + 1) % MAX_AUTHINFO_QUEUE_SIZE != m_Head )
		{
			m_AuthInfos[m_Tail] = Info;
			m_Tail++;
			m_Tail = m_Tail % MAX_AUTHINFO_QUEUE_SIZE;
			return TRUE;
		}

		return FALSE;
	}

	BOOL QueueOut(AuthInfo* pOut)
	{
		AuthInfo	Ret;

		if( m_Tail != m_Head )
		{
			*pOut = m_AuthInfos[m_Head];
			m_Head++;
			m_Head = m_Head % MAX_AUTHINFO_QUEUE_SIZE;
			return TRUE;
		}
		
		return FALSE;
	}

protected:
    AuthInfo	m_AuthInfos[MAX_AUTHINFO_QUEUE_SIZE];
	int			m_Head;
	int			m_Tail;

};

class AuthThread
{
public:
	AuthThread()
	{
		m_bRun		= FALSE;
		m_hThread	= INVALID_HANDLE_VALUE;
		m_nThreadID	= 0;
		m_bUse		= FALSE;

		m_AuthInfo.Init();
	}

	~AuthThread()
	{
		m_bRun = FALSE;
		if(m_hThread != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(m_hThread, 10 * 1000);
			TerminateThread(m_hThread, 1000);

			TRACEF("WaitForSingleObject\n");
		}
	}

public:

	HANDLE	m_hThread;
	UINT	m_nThreadID;

	BOOL	m_bRun;
	BOOL	m_bUse;

	AuthInfo	m_AuthInfo;
};

#endif