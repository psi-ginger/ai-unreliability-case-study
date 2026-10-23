//---------------------------------
// 2005/03/07 성준엽
//---------------------------------
#ifndef _NQUESTDLG_H
#define _NQUESTDLG_H

#include <Directives.h>
#include "Quest/Quest.h"
#include "Skill/Skill.h"
#include "Skill/Skill-Manager.h"

#include "InterfaceMgr/DialogController.h"
#include "Item/ItemCommon/ItemUnit.h"

class HashStringInt;
class cltQuestManager;
class cltCharCommon;
class cltCharClient;
class cltPersonQuestUnitInfo;
class cltQuestMiddleInfo;
class cltQuestClearConditionInfo;
class cltQuestRewardInfo;
class cltQuestInitConditionInfo;
class cltQuestInfoCommon;
class cltQuestInfo;
class cltPQuestInfo;

class CStatic;
class CComboBox;
class CList;
class CButton;
class CEdit;
class CImageStatic;

#include "../../../Common/Quest/Quest.h"

//---------------------------------
// 퀘스트 관리자. 
//---------------------------------
class cltNQuestDlg : public CDialogController
{
public:
	CImageStatic	*m_pPortraitStatic;

	CEdit		*m_pConditionEdit;
	CEdit		*m_pQuestRewardEdit;
	CEdit		*m_pStatusEdit;
	CEdit		*m_pExplainEdit;

	CImageStatic	*m_pSprStatic;

	CButton		*m_pAcceptButton; // 수락
	CButton		*m_pCancelButton; // 취소
	CButton		*m_pOKButton; // 다음

	CStatic		*m_pCondition;
	CStatic		*m_pQuestReward;
	CStatic		*m_pStatus;

	cltNQuestDlg();
	~cltNQuestDlg();

	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show(SI32 kind);
	void Hide();
	void Create();

	void Action();

};

#endif //#if defined(__LEON_START_ALL) || defined(__LEON_NMASTER__) && defined(_DEBUG)







