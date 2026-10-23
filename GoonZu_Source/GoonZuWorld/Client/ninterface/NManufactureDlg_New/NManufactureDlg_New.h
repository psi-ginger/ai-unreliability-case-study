/* ==========================================================================
클래스 :		CNManufactureDlg_New

작성일 :		09/12/16
작성자 :		손성웅

변경사항 :

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CEachPersonTradeOrderInfo;
class CLibListMgr;


class CNManufactureDlg_New : public CDialogController
{
public:
	
	CNManufactureDlg_New();
	~CNManufactureDlg_New();

	void Initialize();
	void Destroy();

	void Create();

	static	void CALLBACK StaticNManufactureDlg_NewProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NManufactureDlg_NewProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, TCHAR *Name, TCHAR *AccountID, BOOL bShopStarted );
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetMakeableItemList();

	void ShopInit(BOOL bSelf);

	void StartPersonalShop();

	void DisplayMessage(BOOL siSuccess, TCHAR *pBuffer);


	void MakeShopItem();
	bool DropShopItem();

	void StopPersonalShop();
	void RefreshShowData();
	void SetItemImageData(SI32 ItemUnique);

	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	bool CheckMakeData_Client()		;					// 만든다고 하기 전에 데이터를 체크 한다.
	bool CheckMakeData_Maker()		;					// 만들겠다고 하기 전에 데이터를 체크 한다.

	void SetComboBox_ItemCategory()	;
	void SetComboBox_ItemCategory_Select();
	
	bool ChekMakeItem_By_ItemType(SI32 siItemtype);	// 해당 항목이 자신이 만들수 있는 아이템이 있는지 확인 한다.
	bool CeckHaveTool( SI32 siItemUnique )	;		// 목표 아이템을 만드는 툴은 가지고 있는가 ?
	bool CeckHaveSkill( SI32 siItemUnique )	;		// 목표 아이템을 만드는 스킬은 가지고 있는가 ?

	bool ChekMakeItem( cltItemInfo* pinfo )	;
	void Set_Manufacture_Mode()				;		// 제작자 모드로 셋팅
	void Set_Client_Mode()				;		// 제작의뢰자 모드로 셋팅
	void MakeItemDataSet(SI32 siItemUnique)	;	// 만들 아이템 데이터를 셋팅한다.
	void SetEditBox();							// 에티트 박스에 데이터들을 넣는다 .
	bool CheckItemType( SI32 siType )		;
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

	bool			m_bClient	;

};
