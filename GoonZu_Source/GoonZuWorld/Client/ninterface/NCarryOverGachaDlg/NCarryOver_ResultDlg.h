
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/Item/ItemCommon/ItemUnit.h"


class CNCarryOver_ResultDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory	m_InterfaceFactory;

	TCHAR*				m_pText;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNCarryOver_ResultDlg( void );
	~CNCarryOver_ResultDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNCarryOver_ResultDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNCarryOver_ResultDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );
	void DrawTitleText( SI32 ScreenRX, SI32 ScreenRY );

public:
	void OnClickReStart( void );
	void OnClickBack( void );

public:
	void SetGachaGetProduct( cltItem* pclItem, bool bGetRareItem );

	void SetInfo( SI32 siGachaItemUnique );


};