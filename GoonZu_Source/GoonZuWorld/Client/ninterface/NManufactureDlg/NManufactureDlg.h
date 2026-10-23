/* ==========================================================================
클래스 :		CNManufactureDlg

작성일 :		06/06/19
작성자 :		김영진

변경사항 :		제조의뢰 인터페이스 분리

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CEachPersonTradeOrderInfo;
class CLibListMgr;


class CNManufactureDlg : public CDialogController
{
public:
	
	CNManufactureDlg();
	~CNManufactureDlg();

	void Initialize();
	void Destroy();

	void Create();

	static	void CALLBACK StaticNManufactureDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NManufactureDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, TCHAR *Name, TCHAR *AccountID, BOOL bShopStarted );
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetMakeableItemList();

	bool AddShopMakeItem();

	void ShopInit(BOOL bSelf);

	void StartPersonalShop();

	void DisplayMessage(BOOL siSuccess, TCHAR *pBuffer);


	bool MakeShopItem();
	bool DropShopItem();

	void StopPersonalShop();
	void RefreshShowData();
	void SetItemImageData(SI32 ItemUnique);

	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
public:

	InterfaceFactory	m_InterfaceFactory;


	BOOL			m_bSelf;
	BOOL		    m_bShopStarted;  
	BOOL			m_bShow;	

	cltItem			m_clUserItem[ MAX_ITEM_PER_PERSON ];

	BOOL				m_bShopDataChanged;
	TCHAR				m_szShopBanner[MAX_PERSONAL_SHOP_TEXT_SIZE];

	SI32				m_siShopTradeIndex[MAX_SHOP_ITEM_NUMBER];	

	cltShopTradeData	m_clShopData;

	SI32			m_siShopperCharUnique;
	SI32			m_siShopperPersonID;
	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	TCHAR			m_szCharName[MAX_PLAYER_NAME];

	SI32			m_siImageType;
	SI32			m_siImageIndex;

};
