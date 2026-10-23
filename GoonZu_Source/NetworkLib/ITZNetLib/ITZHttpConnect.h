#ifndef _ITZHTTPCONNECT_H
#define _ITZHTTPCONNECT_H

#pragma comment(lib, "winhttp.lib")

#include "itzbase.h"
#include <winhttp.h>

class ITZHttpConnect
{
public:
	ITZHttpConnect();
	~ITZHttpConnect();

	void				Init();
	void				Init( SI32 recvUnitBufferSize, SI32 resourceBufferSize );
	void				HttpOpen();
	BOOL				SendRequest( LPWSTR szURL );
	
	void				Cleanup();
	BOOL				QueryHeader();
	BOOL				QueryData();
	void				CompleteRecvData( LPSTR lpReadBuffer, DWORD dwBytesRead );
	BOOL				ReadData( DWORD dwSize );

	BOOL				IsComplete() { return m_bComplete; };
	
public:
	HINTERNET			m_hSession;						// Session handle
	HINTERNET			m_hConnect;						// Connection handle
	HINTERNET			m_hRequest;						// Resource request handle

	DWORD				m_dwMaxRecvUnitBufferSize;		
	DWORD				m_dwMaxResourceBufferSize;		

	char*				m_pRecvUnitBuffer;			
	char*				m_pResourceBuffer;		

	DWORD				m_dwTotalSize;

	BOOL				m_bComplete;
};

#endif