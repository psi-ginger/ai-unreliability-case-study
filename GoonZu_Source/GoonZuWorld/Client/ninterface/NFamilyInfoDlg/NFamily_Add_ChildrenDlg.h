// 자식이름을 받아 등록 시킨다
#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNFamily_Add_ChildrenDlg : public CDialogController
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
	CNFamily_Add_ChildrenDlg( void );
	~CNFamily_Add_ChildrenDlg( void );

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogAddChildrenoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogAddChildrenDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
public:

};