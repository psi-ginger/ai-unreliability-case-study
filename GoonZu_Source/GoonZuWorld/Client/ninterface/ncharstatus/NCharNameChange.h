//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNCharNameChange

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_CHARSTATUS_PAGE_NUMBER	5

#define WANTIED_ANIMAL_CHANGE_TYPE	2

class cltCharClient;
class cltParametaBox;
class cltSummonScroll;
class cltCharManager;
class cltItem;

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;

// 캐릭터 이름 변경 창
class cltNCharNameChange : public CDialogController
{
public:
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	CEdit			*m_pEditbox_explain;
	CStatic			*m_pStatictext_changename;
	CEdit			*m_pEditbox_changename;
	CButton			*m_pButton_changename;
	CButton			*m_pButton_cancel;

	cltNCharNameChange();
	~cltNCharNameChange();

	void Create();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK NCharNameChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NCharNameChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

};

//#endif
