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

#include "../../../Server/ServerReward/ServerReward.h"

class CInterfaceFile;	

class NPromoter_InfoDlg : public CDialogController
{
public:

	NPromoter_InfoDlg();
	~NPromoter_InfoDlg();

	void 	Init( CInterfaceFile* _pfile );

	void 	Create();								// 순수하게 이 창만 사용.
	void	Create( CControllerMgr *pParent );		// 추천인 NPC의 상태창에 탭으로 추가될때 사용한다.

	void	VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	void 	Destroy();
	void 	Action();

	void	SetPotrait( void );
	void	RequestPromoter( void );
	void	SetPromoterInfo( SI16 _siKind, SI16 _siLevel, bool _bAuth, TCHAR* _pszPromoterCharName );
	void	SetGiveItem( SI32* _pItemUnique, SI32* _pItemNum );

	void	Hide();
	void	Show();

	
	static void CALLBACK StatiNPromoter_InfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPromoter_InfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

   
private:
	InterfaceFactory	m_InterfaceFactory;
	
	SI32				m_siImageType;
	SI32				m_siImageIndex;
	SI32				m_GiveItemUnique[Const_Max_RecommandReward_Item];

	char 				m_promoterCharid[NHNCHINAPROMOTER_PROMOTER_CHARID_SIZE];

	bool				m_bChildCreate;	// 이 창이 활성화 되는 경우가 2가지다. NRPBASE_DLG의 자식창일 경우와 아닌경우.
};
 