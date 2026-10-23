/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-System.h"

#include "../../Promoter/NHNChinaPromoter.h"

class NPromoter_Give_Item_Notice : public CDialogController
{
public:
	enum{ NONE, PERSONAL_INFO_CHANGE, WHIPERER };
	enum{ GIVE_ITEM_SUCCESS, GIVE_ITEM_FAIL };

public:

	NPromoter_Give_Item_Notice();
	~NPromoter_Give_Item_Notice();

	void 	Create();
	void 	Destroy();
	void 	Action();

	void	SetText( bool _bGiveItemSuccess );
	void	SetRecommendText( SI32 _bGiveItemSuccess, const TCHAR* _pTempName, const TCHAR* _pItemName, const TCHAR* _pItemNum  );
	void	SetRecommendeeText( SI32 _bGiveItemSuccess, const TCHAR* _pRecommendName, const TCHAR* _pItemName, const TCHAR* _pItemNum );

	static void CALLBACK StatiNPromoter_Give_Item_NoticeProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPromoter_Give_Item_NoticeProc( UINT nEvent, int nControlID, CControllerObj* pControl );

   
private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siButtonType;		
	TCHAR				m_NewbieID[MAX_PLAYER_NAME];
};
 