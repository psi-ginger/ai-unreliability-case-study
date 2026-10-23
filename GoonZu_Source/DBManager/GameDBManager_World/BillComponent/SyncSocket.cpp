// SyncSocket.cpp: implementation of the CSyncSocket class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "SyncSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CSyncSocket::GetLine(char* lpszString)
{
	return GetText(lpszString, -1);
}

int CSyncSocket::GetText(char* lpszString, int nLen)
{
	if (nLen < -1 || MAX_PACKETSIZE <= nLen) 
		return PLPAY_SOCKERR_230;

	lpszString[0] = NULL;

	int ret = 0;

	FD_ZERO(&m_fdread); 
	FD_SET(m_hSocket, &m_fdread);

	ret = Read(lpszString, nLen);
	FD_CLR(m_hSocket, &m_fdread);

	return ret;
}

int	CSyncSocket::Read(void *pData, int nReqLen)
{
	int		nRet = -1;
	int		nRecvedLen = 0;
	int		nRevcBytes = 0;
	char*	pcData = (char* )pData;
	while (nRecvedLen < MAX_PACKETSIZE)
	{
		nRet = select(0, &m_fdread, NULL, NULL, &m_Cmdtimeout);
		if (nRet == SOCKET_ERROR)
			return PLPAY_SOCKERR_240;
		else if (nRet == 0) // timeout
			return PLPAY_SOCKERR_270;

		nRevcBytes = ::recv(m_hSocket, pcData+nRecvedLen, 1, 0);
		if (nRevcBytes > 0) // recv Success
		{				
			nRecvedLen++;
			if (nReqLen > 0 && nReqLen == nRecvedLen)
			{
				pcData[nRecvedLen] = '\0';
				return nRecvedLen;
			}

			if (nReqLen == -1 && nRecvedLen > 2)
			{
				if (pcData[nRecvedLen-2] == '\r' && 
					pcData[nRecvedLen-1] == '\n')
				{
					pcData[nRecvedLen-2] = '\0';
					return nRecvedLen-2;
				}
			}
		}
		else if (nRevcBytes == 0) // socket closed from server
		{
			return PLPAY_SOCKERR_250; //socket close
		}
		else // recv error
		{
			return PLPAY_SOCKERR_250; //socket close
		}
	}

	return nRecvedLen;
}

BOOL CSyncSocket::SendLine(char*  lpszString, int nLen)
{
	return Write(lpszString, strlen(lpszString));
}

int	CSyncSocket::Write(const void *pData, int nLen) 
{
	const char* pcData = (const char* )pData;
	int	n = nLen;
	do
	{
		int r1 = ::send(m_hSocket, pcData, n, 0);
		if (r1 == SOCKET_ERROR)
		{
			return WSAGetLastError();
		}
		else if (r1 == 0)
		{
			return PLPAY_SOCKERR_210;
		}
		else if (r1 < 0)
		{
			return PLPAY_SOCKERR_210;
		}
		pcData += r1;
		n -= r1;
	} 
	while (n > 0);

	return nLen;
}

