#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "../Common/Agriculture/Agriculture.h"
#include "..\CommonLogic\Msg\MsgType-Farming.h"
#include "..\CommonLogic\Msg\MsgType-Mining.h"
#include "Char\CharManager\CharManager.h"


#include "..\CommonLogic\Msg\MsgType-Fishing.h"
#include "..\CommonLogic\Msg\MsgType-Agriculture.h"
#include "..\CommonLogic\MsgType-System.h"
#include "../InterfaceMgr/InterfaceMgr.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\SoundOnOffDlg\SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\Interface\FishingDlg\FishingDlg.h"
#include "../Interface/FarmingDlg/FarmingDlg.h"
#include "../NInterface/NFarmingDlg/NFarmingDlg.h"
#include "..\Interface\DrawChat\DrawChat.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "..\NInterface\NFishingDlg\NFishingDlg.h"
#include "../NInterface/NMiningDlg/NMiningDlg.h"
#include "../NInterface/NFarmingDlg/NFarmingDlg.h"

#include "../Common/Agriculture/Mining/MiningMgr.h"
#include "../Common/Farming/FarmingMgr.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN( cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_AgricultureAutoGain *pclInfo = (cltGameMsgResponse_AgricultureAutoGain*)pclMsg->cData;

	switch( pclInfo->siAgricultureType )
	{
	case AGRICULTURE_TYPE_FISH:
		{
			if(m_pDialog[NFISHING_DLG ]!=NULL)

			{
				((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->SetAutoFishing( pclInfo->bAuto );
			}
		
			if ( pclInfo->bAuto )
			{	
				TCHAR* pTitle = GetTxtFromMgr(1231);
				TCHAR* pText = GetTxtFromMgr(1232);
			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
			else
			{		
				TCHAR* pTitle = GetTxtFromMgr(1231);
				TCHAR* pText = GetTxtFromMgr(1233);
			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}	
		}
		break;
	case AGRICULTURE_TYPE_FARM:
		{

		}
		break;
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTS( cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_AgricultureGetProducts *pclInfo = (cltGameMsgResponse_AgricultureGetProducts*)pclMsg->cData;

	switch( pclInfo->siAgricultureType )
	{
	case AGRICULTURE_TYPE_FARM:
		{
			if ( pclInfo->siAddedInvPos > 0 )
			{
				pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siAddedInvPos ].Set( &pclInfo->clLeftItem );
			}
		}
		break;
	case AGRICULTURE_TYPE_MINE:
		{
			if ( pclInfo->siAddedInvPos > 0 )
			{
				pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siAddedInvPos ].Set( &pclInfo->clLeftItem );
			}
		}
		break;
	}
		
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTSNOTIFY( cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_AgricultureGetProductsNotify *pclInfo = (cltGameMsgResponse_AgricultureGetProductsNotify*)pclMsg->cData;

	if ( pclInfo->siCharUnique < 1 || pclInfo->siItemUnique < 1 )
	{
		return;
	}

	id = cltCharCommon::CharUniqueInfo[ pclInfo->siCharUnique ];

	if ( id < 1 || pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharClient *pclCharClient = (cltCharClient *)pclCM->CR[id];
	
	if ( pclCharClient == NULL )
	{
		return;
	}

	TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siItemUnique );
	if ( ItemName == NULL )
	{
		return;
	}

	TCHAR buffer[ 128 ];

	switch( pclInfo->siAgricultureType )
	{
	case AGRICULTURE_TYPE_FARM:
		{
			TCHAR* pText = GetTxtFromMgr(1234);

			TCHAR num[128]= TEXT(" ") ;
			StringCchPrintf(num, 128, TEXT("%d"),pclInfo->siItemNum);
		
			Nsprintf(buffer,pText,TEXT("item"), ItemName   ,TEXT("num"),  num  ,NULL);
			//sprintf( buffer, pText, ItemName, pclInfo->siItemNum );
			pclCharClient->pclDrawChat->Set( pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );

			// 농경정보 창에 어떤 농산물을 얼마나 얻었는지 표시해준다. (PCK : 08.03.26)
			// 나한테만 정보를 뿌려준다.
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				if(pclCM->CR[1]->GetCharUnique() == pclInfo->siCharUnique)
				{
					if(m_pDialog[NFARMING_DLG ]!=NULL)
					{
						((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->SetFarmingInfo(ItemName, pclInfo->siItemNum);
						((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->m_dwFarmingStartClock = pclClient->CurrentClock;
					}
				}
			}
		}
		break;
	case AGRICULTURE_TYPE_MINE:
		{
			TCHAR* pText = GetTxtFromMgr(3207);
			TCHAR num[128]= TEXT(" " );

			StringCchPrintf(num, 128, TEXT("%d"),pclInfo->siItemNum);
			Nsprintf(buffer,pText,TEXT("item"), ItemName   ,TEXT("num"),  num  ,NULL);

			//sprintf( buffer, pText, ItemName, pclInfo->siItemNum );
			pclCharClient->pclDrawChat->Set( pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );

			// 채광정보 창에 어떤 광물을 얼마나 얻었는지 표시해준다. (PCK : 08.03.26)
			// 나한테만 정보를 뿌려준다.
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				if(pclCM->CR[1]->GetCharUnique() == pclInfo->siCharUnique)
				{
					if(m_pDialog[NMINING_DLG ]!=NULL)
					{
						((CNMiningDlg*)m_pDialog[NMINING_DLG ])->SetMiningInfo(ItemName, pclInfo->siItemNum);
						((CNMiningDlg*)m_pDialog[NMINING_DLG ])->m_dwMiningStartClock = pclClient->CurrentClock;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTURESTART( cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_AgricultureStart *pclInfo = (cltGameMsgResponse_AgricultureStart*)pclMsg->cData;

	switch( pclInfo->siStructureType )
	{
	case RANKTYPE_LAND:
		{
			if(m_pDialog[ NFARMING_DLG ]==NULL)
			{
				CreateInterface(NFARMING_DLG);
			}
			((CNFarmingDlg*)m_pDialog[ NFARMING_DLG ])->SetFarmingItem( pclInfo->siItemUnique, pclInfo->siDelayTime, pclInfo->siSuccessPercent );
			((CNFarmingDlg*)m_pDialog[ NFARMING_DLG ])->m_uiLeaseLeftTime = pclInfo->uiLeaseLandExpire;
			
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->m_dwFarmingStartClock = pclClient->CurrentClock;
				if(((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->ReturnAutoFarming() == TRUE)
				{
					((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->Init();
				}
			}
		}
		break;
	case RANKTYPE_MINE:
		{
			if(m_pDialog[ NMINING_DLG ]==NULL)
			{
				CreateInterface(NMINING_DLG);
			}
		
			((CNMiningDlg*)m_pDialog[ NMINING_DLG ])->SetMiningItem( pclInfo->siItemUnique, pclInfo->siSuccessPercent, pclInfo->siDelayTime );
			((CNMiningDlg*)m_pDialog[ NMINING_DLG ])->m_uiLeaseLeftTime = pclInfo->uiLeaseLandExpire;
			
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				((CNMiningDlg*)m_pDialog[ NMINING_DLG ])->m_dwMiningStartClock = pclClient->CurrentClock;
				if(((CNMiningDlg*)m_pDialog[NMINING_DLG ])->ReturnAutoMining() == TRUE)
				{
					((CNMiningDlg*)m_pDialog[NMINING_DLG ])->Init();
				}
			}
		}
		break;
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTUREPAYRENTFEE( cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_AgriculturePayRentFee *pclInfo = (cltGameMsgResponse_AgriculturePayRentFee*)pclMsg->cData;
	
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftMoney );

	TCHAR *pTitle = NULL;
	TCHAR *pText = NULL;

	switch( pclInfo->siRankType )
	{
	case RANKTYPE_LAND:
		pTitle = GetTxtFromMgr(1262);
		pText = GetTxtFromMgr(1263);
		break;
	case RANKTYPE_MINE:
		pTitle = GetTxtFromMgr(3256);
		pText = GetTxtFromMgr(3257);
		break;
	default:
		return;
	}

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclInfo->siRentFee );

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTUREFORNOTRENTMAN(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_AgricultureForNotRentMan *pclInfo = (cltGameMsgResponse_AgricultureForNotRentMan*)pclMsg->cData;

	if ( pclInfo->siVillageUnique < 1 || pclInfo->siRentPrice < 1 )
	{
		return;
	}

	TCHAR *VillageName = pclVillageManager->GetName( pclInfo->siVillageUnique );
	if ( VillageName )
	{
		switch( pclInfo->siRankType )
		{
		case RANKTYPE_LAND:
			{						
				if(m_pDialog[ NFARMING_DLG ])
				{
					((CNFarmingDlg*)m_pDialog[ NFARMING_DLG ])->LentLand(pclMsg, VillageName, pclInfo->siRentPrice, pclInfo->siItemUnique);
				}
			}
			break;
		case RANKTYPE_MINE:
			{
				if( m_pDialog[NMINING_DLG] == NULL )					break;

				TCHAR Buffer[ 256 ] = TEXT("");
				TCHAR* pText = GetTxtFromMgr(3258);
				TCHAR money[128]= TEXT(" ") ;

				StringCchPrintf(money, 128, TEXT("%d"),pclInfo->siRentPrice);
				Nsprintf(Buffer, pText, TEXT("village1"), VillageName ,TEXT("village2"),  VillageName  ,TEXT("money"),   money ,NULL);
				pText = GetTxtFromMgr(3259);

				stMsgBox MsgBox;
				MsgBox.Set( m_pDialog[NMINING_DLG], pText, Buffer, MSGBOX_TYPE_YESNO, 0 );
				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( pclInfo->siRentPrice );
				memcpy( TempBuffer, &pclInfo->siRentPrice, Size );
				SetMsgBox( &MsgBox, TempBuffer, Size );
			}
			break;
		}
	}		

	return;
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_AGRICULTURELEASELEFTTIME(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_AgricultureLeaseLeftTime* pclinfo = (cltGameMsgResponse_AgricultureLeaseLeftTime*)pclMsg->cData;

	switch( pclinfo->siType )
	{
	case RANKTYPE_MINE:
		{
			if ( m_pDialog[NMINING_DLG] )
			{
				if( pclinfo->siLeftTime > 0)
				{
					((CNMiningDlg*)m_pDialog[NMINING_DLG])->m_uiLeaseLeftTime = pclinfo->siLeftTime;
					((CNMiningDlg*)m_pDialog[NMINING_DLG])->SetMiningExplain();
				}
			}
		}
		break;
	case RANKTYPE_LAND:
		{
			if ( m_pDialog[NFARMING_DLG] )
			{
				if( pclinfo->siLeftTime > 0)
				{
					((CNFarmingDlg*)m_pDialog[NFARMING_DLG])->m_uiLeaseLeftTime = pclinfo->siLeftTime;
					((CNFarmingDlg*)m_pDialog[NFARMING_DLG])->SetFarmingExplain();
				}
			}
		}
		break;
	default:
		break;
	}
}