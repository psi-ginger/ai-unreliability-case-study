//---------------------------------
// 2005/03/16 성준엽
//---------------------------------
#ifndef _NBOBUSANGDLG_H
#define _NBOBUSANGDLG_H

#include <Directives.h>
#include "Quest/Quest.h"
#include "GameEvent/GameEvent.h"

#include "InterfaceMgr/DialogController.h"
#include "Item/ItemCommon/ItemUnit.h"

class CStatic;
class CComboBox;
class CList;
class CButton;
class CEdit;
class CImageStatic;
class CListView;

#include "../../../Common/Quest/Quest.h"

struct stBobusang
{
	SI32			siType;

	bool			bNewAccount;
	bool			bGameEvent;
	bool			bWinPrize;

	SI32			siFlagNum;
	cltItem			clitem;
};

//---------------------------------
// 퀘스트 관리자. 
//---------------------------------
class cltNBobusangDlg : public CDialogController
{
public:
	CImageStatic	*m_pPortrait;

	CEdit		*m_pEdit;

	CEdit	*m_pListView;

	CButton		*m_pOKButton; // 수락

///////////////////////////////////////////////////////////////////////////////////////////////////
// 추가
	CButton*	m_pButton[3];
//////////////////////////////////////////////////////////////////////////////////////////////////
	cltNBobusangDlg();
	~cltNBobusangDlg();

	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();

	void SetData( void *pData );

};

#endif //#if defined(__LEON_START_ALL) || defined(__LEON_NMASTER__) && defined(_DEBUG)







