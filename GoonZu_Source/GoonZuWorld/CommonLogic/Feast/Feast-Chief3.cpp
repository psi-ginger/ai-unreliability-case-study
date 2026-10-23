#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"

#include "Feast.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;

BOOL cltFeastManager::IsMyInventoryChanged()
{

	int i, j;
	char* src;
	char* dst;

	for ( j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		src = (char*)&m_pclMyItem[ j ];
		dst = (char*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

		for( i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


void cltFeastManager::LoadMyInventory( HWND hDlg )
{
	for( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_pclMyItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}
	
	SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_INVENTORY, NULL, &pclClient->pclCM->CR[1]->pclCI->clCharItem );

	return;
}

BOOL cltFeastManager::IsStgChanged()
{
	for ( int i = 0; i < MAX_FEAST_STG_NUMBER; ++i )
	{
		if ( m_pclStgItem[ i ].siItemNum != clClientStrInfo.clFeastStrInfo.clFeastStg[ i ].siItemNum || 
			 m_pclStgItem[ i ].siItemUnique != clClientStrInfo.clFeastStrInfo.clFeastStg[ i ].siItemUnique )
		{
			return true;
		}
	}	
	
	return false;
}

void cltFeastManager::LoadFeastStg( HWND hDlg )
{
	for( int i = 0; i < MAX_FEAST_STG_NUMBER; ++i )
	{
		m_pclStgItem[i].Set( &clClientStrInfo.clFeastStrInfo.clFeastStg[ i ] );
	}

	SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_FEASTSTG, NULL, m_pclStgItem, MAX_FEAST_STG_NUMBER, true );

	return;
}


