// [영훈] 개편된 퀘스트 알림창
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"

class CList;
class CButton;

const SI32 Const_Max_Category_Button	= 3;
const SI32 Const_Max_ListBox_Item		= 19;

class CNQuestInfoDlg_AllQuest : public CDialogController
{
	// 변수영역
private:

	CList*				m_pclList;
	CButton*			m_pclButton[Const_Max_Category_Button];
	SI16				m_siButtonPosY[Const_Max_Category_Button];

	// 함수영역
public:
	CNQuestInfoDlg_AllQuest();
	~CNQuestInfoDlg_AllQuest();

	void Create(CControllerMgr* pParent);

	static void CALLBACK StaticQuestInfoDlg_AllQuestProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NQuestInfoDlg_AllQuestProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void Show();
	void Hide();


private:
	void SelectCategory( SI32 siControlID );
	void ChangeList( SI32 siIndex );
	void CalcListHeight( void );


};
