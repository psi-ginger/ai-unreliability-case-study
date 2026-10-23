#include "StructureRepairDlg.h" 
#include <CommonLogic.h>
#include "..\..\Server\Server.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../CityHall/TaxpaymentListDlg.h"


extern cltCommonLogic* pclClient;

cltStructureMngBasic*	cltStructureRepairDlg::m_pclStructureManager;



cltStructureRepairDlg::cltStructureRepairDlg()
{
	m_pclStructureManager = NULL;

	m_pclMyItem = new cltItem[ MAX_ITEM_PER_PERSON ];

}

cltStructureRepairDlg::~cltStructureRepairDlg()
{
	if ( m_pclMyItem ) delete [] m_pclMyItem;

}

void cltStructureRepairDlg::Create( HINSTANCE hInst, HWND hParent, cltStructureMngBasic *pclStructureManager )
{

	m_pclStructureManager = pclStructureManager;

}

BOOL cltStructureRepairDlg::IsMyInventoryChanged()
{

	int i, j;
	BYTE* src;
	BYTE* dst;

	for ( j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		src = (BYTE*)&m_pclMyItem[ j ];
		dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

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



