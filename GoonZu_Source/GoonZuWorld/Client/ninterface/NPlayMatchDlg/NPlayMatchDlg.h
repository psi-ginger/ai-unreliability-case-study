/* ==========================================================================
	클래스 :		NPlayMatchDlg

	작성일 :		05/07/22
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;

class NPlayMatchDlg : public CDialogController
{
public:


	CStatic     		*m_pNNPCMenuStatic1;
	CStatic     		*m_pNNPCMenuStatic2;

	CButton 			*m_pButton_blackarmy_apply;
	CButton 			*m_pButton_blackarmy_mvp;

	CButton				*m_pButton_Dungeon;
	CButton				*m_pButton_Battle;

	CButton				*m_pButton_HuntingMapInfo;	// 소유 사냥터 정보
	CButton				*m_pButton_BattleGuildSet;	// 사냥터 소유권 신청.

	NPlayMatchDlg();
	~NPlayMatchDlg();

	void Create();
	//void RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen );

	static void CALLBACK NPlayMatchStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPlayMatchProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();
	void SetNPCText(TCHAR *text);

};

//#endif
