
#pragma once

#include "../../InterfaceMgr/DialogController.h"



/*
*/

class CNCharHouseDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNCharHouseDlg( void );
	~CNCharHouseDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNCharHouseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNCharHouseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();

	void RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetCharHouseList( void );

public:
	void OnButtonClick_Change( void );
	void OnButtonClick_Delete( void );

};