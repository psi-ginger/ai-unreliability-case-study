/* ==========================================================================
클래스 :		NBeMemberDlg

작성일 :		06/11/01
작성자 :		김영진

변경사항 :		없음

차후작업 :		

========================================================================*/

#pragma once

#include "../../StrBaseDlg/StrBaseDlg.h"

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"



class		CButton;
class		CEdit;
class		CList;

class NBeGuildMemberDlg : public CDialogController
{
public:

	NBeGuildMemberDlg();
	~NBeGuildMemberDlg();

	static	void CALLBACK NBeGuildMemberStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NBeGuildMemberProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();

	void RefreshList();

	bool m_bShow;

	static cltStructureMngBasic*	m_pclStructureManager;

	CButton	*m_pButton_GuildMember	;
	CButton	*m_pButton_delGuildMember	;
	CEdit	*m_pEdit_name	;
	CList	*m_pList_GuildMember	;

	void RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

