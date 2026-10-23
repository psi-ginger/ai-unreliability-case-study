#ifndef __SYNCSOCKET_H_
#define __SYNCSOCKET_H_

#include "BillSocket.h"

class CSyncSocket : public CBillSocket  
{
	fd_set  m_fdread; 

public:
	CSyncSocket(SOCKET h = INVALID_SOCKET): CBillSocket(h) {}
	int	Read(void *pData, int nLen); 
	int	Write(const void *pData, int nLen); 
	BOOL SendLine(char*  lpszString, int nLen);
	BOOL GetText(char *lpszString, int nLen);
	BOOL GetLine(char *lpszString);
};
#endif	// __SYNCSOCKET_H_
