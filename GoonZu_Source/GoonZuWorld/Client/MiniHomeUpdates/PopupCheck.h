//=====================================================================
// PopupCheck.h
//=====================================================================

#ifndef _POPUPCHECK_H_
#define _POPUPCHECK_H_

#include "ITZHttpConnect.h"

class cltPopupCheck
{
public:
	cltPopupCheck();
	~cltPopupCheck();

	BOOL				GetRequest(void)	{ return m_bRequest; }
	void				Request_PopupCheck( TCHAR* pszIP );
	BOOL				CheckUpdate_PopupCheck();
	BOOL				m_bPCBang;
	BOOL				m_bPopup;

private:
	BOOL				m_bRequest;

public:
	ITZHttpConnect		m_httpconnect;
	TCHAR				m_FullURL[256];
};

#endif