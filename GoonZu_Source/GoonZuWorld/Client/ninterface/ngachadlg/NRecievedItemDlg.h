/* ==========================================================================
클래스 :		CNGachaDlgNew

작성일 :		07/04/11
작성자 :		김영진

변경사항 :		가차(뽑기) 시스템 인터페이스

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../server/GachaManager/GachaManager.h"


class CNRecievedItemDlg : public CDialogController
{
public:

	CNRecievedItemDlg();
	~CNRecievedItemDlg();

	void Create();
	void Destroy();

	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticNRecievedItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NRecievedItemDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void Action();	

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
public:
	void SetRecieveItem( cltItem * pclItem , UI16 GachaCnt , bool BestProduct  );
	bool CheckHaveKey( SI32 GachaUseKeyKind );

private:

	InterfaceFactory	m_InterfaceFactory;
		
	SI32				m_siGachaUseKeyKind;		// 어떤 열쇠를 사용하느냐

};
 