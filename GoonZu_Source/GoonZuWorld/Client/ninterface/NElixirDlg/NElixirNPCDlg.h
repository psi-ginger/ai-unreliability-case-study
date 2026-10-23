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

class NElixirNPCDlg : public CDialogController
{
public:

	NElixirNPCDlg();
	~NElixirNPCDlg();

	void Init();

	void Create();
	void Destroy();
	void Action();

	static void CALLBACK StatiNElixirNPCDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NElixirNPCDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void SetItemListInfo( SI32* siItemUnique, SI32* siWaterNum );	// 콤보박스에 해당하는 아이템의 정보를 서버에서 받아 리스트박스에 세팅.
	void SetMyInventory();											// 내 인벤토리의 콤보박스에 해당하는 종류의 아이템 추가하귀~
	void SetSelectItem();											// 리스트 박스에 선택된 아이템 이미지를 보이기위해 리스트뷰에 추가.

private:
	InterfaceFactory	m_InterfaceFactory;

	bool				m_bClickSelectItem;
	SI32				m_siComboBoxItemtype;
	SI32				m_siSelectChangeItemInvenPos;

	SI32				m_siItemUnique[ELIXIR_LIST_NUM]; 
	SI32				m_siWaterNum[ELIXIR_LIST_NUM];
};
 