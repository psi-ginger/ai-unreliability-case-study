//---------------------------------
// 2003/6/18 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Market.h"
#include "Control.h"
#include "..\..\Client\FontManager\FontManager.h"
#include "..\..\Client\Music\Music.h"
#include <JYLibFunction.h>

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;


// 객주 창고를 설정한다. 
/*BOOL cltMarketManager::SetMarketStorage(HWND hDlg, SI32 id)
{
	LVITEM LI;
	SI32 i;
	char buffer[256];

	// 이미지 리스트를 리스트 뷰 컨트롤에 등록한다. 
	HWND hwnd = GetDlgItem(hDlg, IDC_LIST_MARKETSTORAGE);

	// 이미지 리스트가 리스트 뷰와 연결되어 있지 않으면, 
	// 연결 시킨다. 
	if(ImageListConnectedToStorageSwitch == FALSE)
	{
		ImageListConnectedToStorageSwitch = TRUE;

		if(pclImageList)
		{
			pclImageList->ConnectToListView(hwnd, LVSIL_NORMAL);
		}
	}

	// 이미 업데이트 되어 있다면 추가로 업데이트 하지 않는다. 
	if(clOldAccount.IsSame(&pclCM->CR[id]->pclCI->clMarket.clInfo) == TRUE)
	{
		return TRUE;
	}
	else
	{
		clOldAccount.Set(&pclCM->CR[id]->pclCI->clMarket.clInfo);
	}

	// 기존의 항목을 모두 삭제한다. 
	ListView_DeleteAllItems(hwnd);

	// 리스트 뷰 컨트롤에 항목을 추가한다. 
	LI.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	LI.state=0;
	LI.stateMask=0;
	
	SI32 index = 0;
	for(i = 0;i < MAX_ORDER_PER_PERSON;i++)
	{
		cltItem* pclitem = &pclCM->CR[id]->pclCI->clMarket.clInfo.clItem[i];

		if(pclitem->siUnique)
		{

			LI.iImage	= pclitem->GetImage(pclClient->pclItemManager);
			LI.iSubItem	= 0;
			LI.iItem	= index;
			
			char itemname[128];
			if(pclitem->GetFullName(pclClient->pclItemManager, itemname) == true)
			{
				
				sprintf(buffer, "%s %d개", itemname, pclitem->GetItemNum());
				LI.pszText	= buffer;
				LI.lParam	= i;
				index++;
				
				ListView_InsertItem(hwnd, &LI);
			}
		}
	}

	return TRUE;

}*/

/*BOOL cltMarketManager::SetInventory(HWND hDlg, SI32 id)
{
//	LVITEM LI;
//	SI32 i;
//	char buffer[256];
	
	if(id <= 0)return FALSE;


	// 캐릭터의 인벤토리를 찾는다. 
	cltPItemInfo* pcliteminfo = &pclCM->CR[id]->pclCI->clCharItem;

	// 예전 인벤토리와 새로운 인벤토리가 같으면, 
	if(clOldPersonItemInfo.IsSame( pcliteminfo, true) == TRUE)
	{
		return FALSE;
	}
	else
	{
		// 현재의 인벤토리를 기억한다. 후에 인벤토리가 변경되었는지 확인해야 하기 때문이다. 
		clOldPersonItemInfo.Set(pcliteminfo);
	}

	if(ImageListConnectedSwitch == FALSE)
	{
		if(SetInventoryInListView(pclClient->pclItemManager, hDlg, IDC_LIST_MARKETINV, pclImageList, pcliteminfo) == TRUE)
		{
			ImageListConnectedSwitch = TRUE;
		}
	}
	else 
	{
		SetInventoryInListView(pclClient->pclItemManager, hDlg, IDC_LIST_MARKETINV, NULL, pcliteminfo);
	}

	return TRUE;

}*/

//주문 내용을 보여준다. 
/*BOOL cltMarketManager::ShowOrderList(HWND hDlg, SI32 id)
{
	SI32 i;
	char buffer[256];

	// 리스트 박스를 초기화한다. 
	SendDlgItemMessage(hDlg, IDC_LIST_MARKETSELLORDER, LB_RESETCONTENT, 0, 0);

	for(i = 0;i < MAX_ORDER_PER_PERSON;i++)
	{
		cltMarketAccount* pclaccount = &pclCM->CR[id]->pclCI->clMarket.clInfo;

		if(pclaccount->clItem[i].siUnique)
		{
			cltItem clitem;
			clitem.Set(&pclaccount->clItem[i]);

			char itemname[128];
			if(clitem.GetFullName(pclClient->pclItemManager, itemname) == true)
			{
				sprintf(buffer, "%s -%d개  개당 %I64d냥",itemname, clitem.siItemNum, pclaccount->clMoney[i].itMoney); 
				SendDlgItemMessage(hDlg, IDC_LIST_MARKETSELLORDER, LB_ADDSTRING, 0, (LPARAM)buffer);
			}
		}
	}


	return TRUE;
}*/

//팔자 버튼을 활성화/비활성화 시킨다.
BOOL cltMarketManager::EnableSellButton(HWND hDlg, SI32 dlgItem, BOOL mode, BOOL letmode)
{
	// 현재 어떤 상태이냐에 관계 없이 강제로 설정하는 모드(letmode == TRUE)가 아니면, 
	if(letmode == FALSE)
	{
		if(SellButtonEnableSwitch == mode)return FALSE;
	}

	SellButtonEnableSwitch = mode;
	HWND hwnd = GetDlgItem(hDlg, dlgItem);
	EnableWindow(hwnd, mode);

	return TRUE;
}

//회수 버튼을 활성화/비활성화 시킨다.
BOOL cltMarketManager::EnableGetFromMarketButton(HWND hDlg, SI32 dlgItem, BOOL mode, BOOL letmode)
{
	// 현재 어떤 상태이냐에 관계 없이 강제로 설정하는 모드(letmode == TRUE)가 아니면, 
	if(letmode == FALSE)
	{
		if(GetFromMarketButtonEnableSwitch == mode)return FALSE;
	}

	GetFromMarketButtonEnableSwitch = mode;
	HWND hwnd = GetDlgItem(hDlg, dlgItem);
	EnableWindow(hwnd, mode);

	return TRUE;
}




// 수수료 정보를 보여준다.
BOOL cltMarketManager::ShowFee(HWND hDlg)
{
	char buffer[256];
	float feerate = (float)clClientStrInfo.clMarketStrInfo.clFee.GetFeeRate() / 10;

	sprintf(buffer, "%3.1f%%", feerate);
	SetDlgItemText(hDlg, IDC_STATIC_MARKETFEE, buffer);

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL cltMarketManager::ShowRealFee(HWND hDlg, SI32 itemnum, SI32 price)
{
	char buffer[256];

	SI32 totalmoney = itemnum * price;

	sprintf(buffer, "%d", clClientStrInfo.clMarketStrInfo.clFee.CalcFee(totalmoney));
	SetDlgItemText(hDlg, IDC_EDIT_MARKETREALFEE, buffer);

	return TRUE;
}

// SJY
BOOL cltMarketManager::IsChangedInvItem()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_InvItem[j];
		char* dst = (char*)&pclCM->CR[1]->pclCI->clCharItem.clItem[j];

		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void cltMarketManager::LoadInvItemData()
{
	for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_InvItem[i].siUnique = 0 ;
		m_InvItem[i].siItemNum = 0 ;

		m_InvItem[i] = pclCM->CR[1]->pclCI->clCharItem.clItem[i];
	}

	return;
}

BOOL cltMarketManager::IsChangedStgItem()
{
	for ( int j = 0; j < MAX_ORDER_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_StgItem[j];
		char* dst = (char*)&pclCM->CR[1]->pclCI->clMarket.clInfo.clItem[j];

		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void cltMarketManager::LoadStgItemData()
{
	for ( int i = 0; i < MAX_ORDER_PER_PERSON; ++i )
	{
		m_StgItem[i].siUnique = 0 ;
		m_StgItem[i].siItemNum = 0 ;

		m_StgItem[i] = pclCM->CR[1]->pclCI->clMarket.clInfo.clItem[i];
	}

	return;
}