#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"
/*
class cltCharClient;
class cltParametaBox;
class cltSummonScroll;
class cltCharManager;
class cltItem;
*/

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;

// 캐릭터 인사말 변경 창
class cltNCharGreetingChange : public CDialogController
{
public:

	TCHAR m_szGreetMessage[128];
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	CEdit			*m_pEditbox_greeting;
	CEdit			*m_pEditbox_newtext;
	CEdit			*m_pEditbox_homepage;
	
	CStatic			*m_pStatictext_1;
	CStatic			*m_pStatictext_2;
	CStatic			*m_pStatictext_3;
	

	CButton			*m_pButton_set;
	CButton			*m_pButton_cancel;

	CButton			*m_pButton_character_change;	
	CButton			*m_pButton_ability_init;
	CButton			*m_pButton_naming; // 네이밍 퀘스트 버튼 추가
	


	cltNCharGreetingChange();
	~cltNCharGreetingChange();

	void Create();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK NCharGreetingChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NCharGreetingChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

	void SetGreetingText(TCHAR* pText);

	void Action();

};
