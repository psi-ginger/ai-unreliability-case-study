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

#define	MAX_GACHA_PRODUCT_ITEM 8

class CNGachaDlgNew : public CDialogController
{
public:

	CNGachaDlgNew();
	~CNGachaDlgNew();

	void Create();
	void Destroy();

	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StatiCNGachaDlgNewProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NGachaDlgNewProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void Action();	

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void SetGachaProductAndCount( cltItem* pclItem, UI16 GachaCnt );
	void SetGachaCount( UI16 _GachaCnt );
	void SetGachaUseKeyKind( SI32 _siGachaUseKeyKind );
	SI32 GetGachaUseKeyKind();
	void SetExplain2(TCHAR* _pText);
	void Text( SI32 ScreenRX, SI32 ScreenRY );
	
private:

	InterfaceFactory	m_InterfaceFactory;
	
	SI32				m_uiGachaCnt;								// 누적된 가차 횟수
	cltItem				m_clGachaProductItem[MAX_GACHA_PRODUCT];	// 가차로 얻을수 있는 아이템.

	TSpr				m_GachaGageImg;
	TSpr				m_NumberImg;

	SI32				m_siGachaUseKeyKind;
	TCHAR*				m_pText;									// 현재 출력할 텍스트 번호.

};
 