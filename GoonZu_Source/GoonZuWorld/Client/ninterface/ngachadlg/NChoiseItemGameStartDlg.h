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

#define MAX_CHOISE_ITEM 4

class CNChoiseItemGameStartDlg : public CDialogController
{
public:

	CNChoiseItemGameStartDlg();
	~CNChoiseItemGameStartDlg();

	void Create();
	void Destroy();

	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticNChoiseItemGameStartDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NChoiseItemGameStartDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void Action();	

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
public:
	bool CheckHaveKey( SI32 GachaUseKeyKind );
	void SetKeyItem( void );

private:

	InterfaceFactory	m_InterfaceFactory;
		
	SI32				m_siGachaUseKeyKind;		// 어떤 열쇠를 사용하느냐

};
 