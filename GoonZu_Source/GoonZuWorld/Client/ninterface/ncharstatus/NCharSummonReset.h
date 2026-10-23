#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CEdit;
class CStatic;

// 캐릭터 인사말 변경 창
class CNCharSummonReset : public CDialogController
{
public:
	CNCharSummonReset();
	~CNCharSummonReset();

	void Create();

	static void CALLBACK NCharSummonResetStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NCharSummonResetProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void ViewStoneResetDlg(TCHAR* pText)	;

private:
	void SendSummonStatusInitMessageToServer();

private:

	CEdit			*m_pEditbox_Info;
	CStatic			*m_pStatictext_1;
	CButton			*m_pButton_ReSet;


};