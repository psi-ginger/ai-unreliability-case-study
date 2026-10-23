#pragma once

#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"
#include "..\../../Client\Inventory\ItemInventoryBaseDefinition.h"

#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

class CStatic;
class CButton;
class CListView;
class CEdit;


class CNItemMallManager1 : public CDialogController
{
public:
	CNItemMallManager1();
	~CNItemMallManager1();

	void					Create();
	void                    ShowItemList();
	void                    SetGetItemInfoTxt(TCHAR * Txt);
	//void					set(SI32 money , SI32 exp, SI16 killmode, TCHAR* pname );

	static void CALLBACK	StatiCNItemMallManager1Proc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NItemMallManager1Proc( UINT nEvent, int nControlID, CControllerObj* pControl );
	BOOL                    IsChangedInvItem();
	void                    LoadInvItemData();
	void                    LoadGetItemData();
	void                    Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );




private:
	


	// 알수없음--; 쿠키넣기
	SI32			m_siViewPage;
	SI32       m_siSelectItem;
	
	DWORD			        m_dwAdviseCookie;

	CStatic                 *m_pNItemMallStatic1;
	CStatic                 *m_pNItemMallStatic2;
	CStatic                 *m_pNItemMallStatic3;
	CStatic                 *m_pNItemMallStatic4;
	CStatic                 *m_pNItemMallStatic5;
	CStatic                 *m_pNItemMallStatic6;
	CStatic                 *m_pNItemMallStatic7;
	CStatic                 *m_pNItemMallStatic8;
	CStatic                 *m_pNItemMallStatic9;
	CButton                 *m_pNItemMallButton1;
	CButton                 *m_pNItemMallButton2;
	CButton                 *m_pNItemMallButton3;
	CButton                 *m_pNItemMallButton4;
	CButton                 *m_pNItemMallButton5;
	CListView               *m_pNItemMallListView1;
	CListView               *m_pNItemMallListView2;
	CEdit                   *m_pNItemMallEdit;

	cltItem               *m_pclUserInvItem;
	cltItem		          *m_GetItem;

	DWORD					 m_dwLastClock;
	//DWORD					 m_dwStartClock;


	
};
