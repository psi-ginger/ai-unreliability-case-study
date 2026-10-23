/* ==========================================================================
클래스 :		CNItemMallProductDlg

작성일 :		06/07/10
작성자 :		김영진

변경사항 :		소지중인 아이템몰 아이템을 보여줌

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


#define MAX_ITEMMALL_ITEM_PER_PAGE	5


class CNItemMallProductDlg : public CDialogController
{
public:

	CNItemMallProductDlg();
	~CNItemMallProductDlg();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticNItemMallProductDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NItemMallProductDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();	
	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	void ListItemMallProduct();
	void ShowItemMallProductList();

	BOOL IsItemMallProduct(cltItem* pclItem);
	SI32 GetAlignValue( cltItem* pclItem ); // 정렬을 위해 정렬의 기준이될 우선순위값을 반환해주는 함수
public:

	InterfaceFactory	m_InterfaceFactory;

	// 아이템 정보를 담는 리스트		
	list< cltItem > m_ItemMallProductList;
	list< cltItem >::iterator m_ItemMallProductListItr;

	SI32 m_siCurrentPage;
	SI32 m_siMaxPage;

	SI32 m_siImageType[MAX_ITEMMALL_ITEM_PER_PAGE];
	SI32 m_siImageIndex[MAX_ITEMMALL_ITEM_PER_PAGE];

};
