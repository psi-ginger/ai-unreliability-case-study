//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNCharKindChange

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

// 캐릭터 종류 변경 창
class cltNCharKindChange : public CDialogController
{
public:
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	SI32 m_siSelectedUnique;

	CButton 			*m_pButton_left;
	CImageStatic 		*m_pImagestatic_portrait;
	CButton 			*m_pButton_right;
	CStatic				*m_pStatictext_name;
	CButton 			*m_pButton_change;
	CButton 			*m_pButton_cancel;

	cltNCharKindChange();
	~cltNCharKindChange();

	void Create();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK NCharKindChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NCharKindChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetPortrait();
	void Close();

};

//#endif
