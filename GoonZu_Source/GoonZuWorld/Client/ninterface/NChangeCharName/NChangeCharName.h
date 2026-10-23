#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;

class CNChangeCharName : public CDialogController
{
private:
	InterfaceFactory	m_InterfaceFactory;

private:
	TCHAR				m_pStrName[MAX_PLAYER_NAME];
	SI08				m_siSlotIndex;

public:
	CNChangeCharName();
	~CNChangeCharName();

	void	Create();

	void	SetCharName(TCHAR* pStrName){ StringCchCopy(m_pStrName, MAX_PLAYER_NAME, pStrName); }
	void	SameName();  // 캐릭터명 동일
	void	SetSlotIndex(SI08 siSlotIndex){m_siSlotIndex = siSlotIndex;}

	static	void CALLBACK StaticNChangeCharNameProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NChangeCharNameProc( UINT nEvent, int nControlID, CControllerObj* pControl );
};
