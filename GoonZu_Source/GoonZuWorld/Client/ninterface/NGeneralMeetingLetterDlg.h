#pragma once

#include "../InterfaceMgr/DialogController.h"
//#include "..\CommonLogic\PostOffice\PostOffice.h"

class CButton;
class CStatic;


// 주주 총회 참여 여부
class NGeneralMeetingLetterDlg : public CDialogController
{
public:

	CStatic*			m_pStatic;
	CStatic*			m_pStatic1;//마을명에 의해 글의 뒷부분이 짤리기에 스태틱 분리. [2007.07.05 손성웅]
	CButton*			m_pButton[2];

	NGeneralMeetingLetterDlg();
	~NGeneralMeetingLetterDlg();

	void Create();

	void InitInfo();
	void AddInfo( int villageunique );
	void SetText();

	static void CALLBACK StaticCallBackDialogNGeneralMeetingLetterDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGeneralMeetingLetterDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	SI32 m_siVillageUnique[ MAX_STOCK_KIND_IN_ACOUNT ];
	SI32 m_curIndex;

};