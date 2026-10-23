
#include "Char/CharManager/CharManager.h"

#include "../Server/Server.h"


#include "../../DBManager/GameDBManager_World/DBMsg-Gacha.h"

#include "../Server/GachaManager/GachaManager.h"
#include "../Server/Gacha2Manager/Gacha2Manager.h"


#include "../../Client/NInterface/ngachadlg/NGachaDlg.h"
#include "../../Client/NInterface/ngachadlg/NGachaDlgNew.h"
#include "../../Client/NInterface/ngachadlg/NRecievedItemDlg.h"

#include "../../Client/NInterface/ngacha2dlg/NGacha2Dlg.h"

#include "../../Client/ninterface/NCarryOverGachaDlg/NCarryOverGachaDlg.h"
#include "../../Client/ninterface/NCarryOverGachaDlg/NCarryOver_ResultDlg.h"


#include "Msg/MsgType-Gacha.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_GACHAPRODUCT_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GachaProduct_List* pclinfo = (cltGameMsgResponse_GachaProduct_List*)pclMsg->cData;
	
	if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
	{
		if( m_pDialog[NGACHANEW_DLG] != NULL )
		{
#ifdef _SAFE_MEMORY
		((CNGachaDlgNew*)m_pDialog[NGACHANEW_DLG])->SetGachaProductAndCount( &pclinfo->clItem[0] , pclinfo->uiGachaCnt );
#else
		((CNGachaDlgNew*)m_pDialog[NGACHANEW_DLG])->SetGachaProductAndCount( pclinfo->clItem , pclinfo->uiGachaCnt );
#endif
		}
		else if( m_pDialog[NGACHA2_DLG] != NULL )
		{
#ifdef _SAFE_MEMORY
		((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( &pclinfo->clItem[0] , pclinfo->uiGachaCnt );
#else
		((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( pclinfo->clItem , pclinfo->uiGachaCnt );
#endif
		}
		else
			return;
	}
	else
	{
		if( m_pDialog[NGACHA_DLG] != NULL ) 
		{
#ifdef _SAFE_MEMORY
		((CNGachaDlg*)m_pDialog[NGACHA_DLG])->SetGachaProductAndCount( &pclinfo->clItem[0] , pclinfo->uiGachaCnt );
#else
		((CNGachaDlg*)m_pDialog[NGACHA_DLG])->SetGachaProductAndCount( pclinfo->clItem , pclinfo->uiGachaCnt );
#endif
		}
		else if(m_pDialog[NGACHA2_DLG] != NULL ) 
		{
#ifdef _SAFE_MEMORY
		((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( &pclinfo->clItem[0] , pclinfo->uiGachaCnt );
#else
		((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( pclinfo->clItem , pclinfo->uiGachaCnt );
#endif
		}
		else
			return;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GACHA2PRODUCT_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Gacha2Product_List* pclinfo = (cltGameMsgResponse_Gacha2Product_List*)pclMsg->cData;
	
	if( m_pDialog[NGACHA2_DLG] != NULL )
	{
#ifdef _SAFE_MEMORY
	((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( &pclinfo->clItem[0] , pclinfo->uiGachaCnt );
#else
	((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaProductAndCount( pclinfo->clItem , pclinfo->uiGachaCnt );
#endif
	}
	else
		return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GACHABUY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GachaBuy* pclinfo = (cltGameMsgResponse_GachaBuy*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
	{
		if( m_pDialog[NGACHA2_DLG] != NULL )
		{
			((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaGetProductAndCount( &pclinfo->clGetItem, pclinfo->uiGachaCnt , pclinfo->bBestProduct );
		}
		else
		{
			if ( NULL == m_pDialog[NCARRYOVERGACHA_DLG] )
			{
				return;
			}

			if( m_pDialog[NOPENBOX] )
			{
				DestroyInterface(m_pDialog[NOPENBOX]);
			}

			if( m_pDialog[NGACHARESULT_DLG] == NULL ) 
			{
				CreateInterface( NGACHARESULT_DLG );
			}
			
			((CNCarryOverGachaDlg*)m_pDialog[NCARRYOVERGACHA_DLG])->SetGachaGage( pclinfo->uiGachaCnt );
			((CNCarryOver_ResultDlg*)m_pDialog[NGACHARESULT_DLG])->SetGachaGetProduct( &pclinfo->clGetItem, pclinfo->bBestProduct );
		}
	}
	else if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
	{
		if( m_pDialog[NGACHA2_DLG] != NULL )
		{
			((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaGetProductAndCount( &pclinfo->clGetItem, pclinfo->uiGachaCnt , pclinfo->bBestProduct );
		}
		else
		{
			if( m_pDialog[NOPENBOX] )  
				DestroyInterface(m_pDialog[NOPENBOX]);

			if( m_pDialog[RECIEVED_ITEM_DLG] == NULL ) 
				CreateInterface( RECIEVED_ITEM_DLG );

			((CNRecievedItemDlg*)m_pDialog[RECIEVED_ITEM_DLG])->SetRecieveItem( &pclinfo->clGetItem, pclinfo->uiGachaCnt , pclinfo->bBestProduct );					
		}
	}
	else
	{
		if( m_pDialog[NGACHA_DLG] != NULL ) 
		{
			((CNGachaDlg*)m_pDialog[NGACHA_DLG])->SetGachaGetProductAndCount( &pclinfo->clGetItem, pclinfo->uiGachaCnt , pclinfo->bBestProduct );
		}
		else if( m_pDialog[NGACHA2_DLG] != NULL ) 
		{
			((CNGacha2Dlg*)m_pDialog[NGACHA2_DLG])->SetGachaGetProductAndCount( &pclinfo->clGetItem, pclinfo->uiGachaCnt , pclinfo->bBestProduct );		
		}
		else
			return;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_CARRYOVER_GACHA_LIST( cltMsg* pclMsg, SI32 id )
{
	if ( false == IsCountrySwitch(Switch_CarryOverGacha) )
	{
		return;
	}

	cltGameMsgResponse_CarryOver_Gacha_list* pclinfo = (cltGameMsgResponse_CarryOver_Gacha_list*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( NULL == m_pDialog[NCARRYOVERGACHA_DLG] )
	{
		return;
	}

	((CNCarryOverGachaDlg*)m_pDialog[NCARRYOVERGACHA_DLG])->SetTotalGachaListAndGachaCount( pclinfo->m_siTotalGachaList, pclinfo->m_uiGachaCnt );
	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_CARRYOVER_SELECTPRODUCT_LIST( cltMsg* pclMsg, SI32 id )
{
	if ( false == IsCountrySwitch(Switch_CarryOverGacha) )
	{
		return;
	}

	cltGameMsgResponse_CarryOver_SelectProduct_List* pclinfo = (cltGameMsgResponse_CarryOver_SelectProduct_List*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( NULL == m_pDialog[NCARRYOVERGACHA_DLG] )
	{
		return;
	}

	((CNCarryOverGachaDlg*)m_pDialog[NCARRYOVERGACHA_DLG])->SetGachaProducts( pclinfo->m_siProductList );

}
