//////////////////////////////////////////////////////////////////////////
//-
//////////////////////////////////////////////////////////////////////////


#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CImageStatic;

class NHalloweenSpiritBoxDlg : public CDialogController
{
private:
	enum
	{
		HALLOWEENSPIRITBOX_1,
		HALLOWEENSPIRITBOX_2,
		HALLOWEENSPIRITBOX_END,
	};

	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siImageType[HALLOWEENSPIRITBOX_END];
	SI32				m_siImageIndex[HALLOWEENSPIRITBOX_END];

	SI32				m_siItemUnique[HALLOWEENSPIRITBOX_END];

	SI08				m_siSelectedIndex;

public:

	NHalloweenSpiritBoxDlg();
	~NHalloweenSpiritBoxDlg();

	SI32	m_siSpritBoxUnique ;	// 스피릿 박스를 삭제 하기 위해서 유니크를 날려야 한다 그래서 선택 하면서 다이얼로그에서 가지고 있는다.

	void	Create();

	static	void CALLBACK StaticNHalloweenSpiritBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NHalloweenSpiritBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void	DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY);
	void	SetSpritItmeUnique(SI32 siItemUnique)	;
};
