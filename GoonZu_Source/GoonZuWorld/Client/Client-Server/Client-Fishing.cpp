#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-Fishing.h"
#include "..\CommonLogic\MsgType-System.h"
#include "..\CommonLogic\CommonLogic.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\SoundOnOffDlg\SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\Interface\FishingDlg\FishingDlg.h"
#include "..\NInterface\NFishingDlg\NFishingDlg.h"
#include "..\Interface\DrawChat\DrawChat.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

void cltClient::DoMsg_GAMEMSG_RESPONSE_FISHINGSTART(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FishingStart *pclInfo = (cltGameMsgResponse_FishingStart*)pclMsg->cData;

	if ( pclInfo->siFishingBaitItemUnique )
	{		
			if(m_pDialog[NFISHING_DLG ]==NULL)
				CreateInterface(NFISHING_DLG);
				
#ifdef _SAFE_MEMORY
			((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->Set( pclInfo->siFishingBaitItemUnique, &pclInfo->clFishingToolList[0] );
#else
			((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->Set( pclInfo->siFishingBaitItemUnique, pclInfo->clFishingToolList );
#endif
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->m_dwFishingStartClock = pclClient->CurrentClock;

				if(((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->ReturnAutoFishing() == FALSE)
				{
					((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->Init();
				}
			}
	}
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FishingDecreaseToolDurability *pclInfo = (cltGameMsgResponse_FishingDecreaseToolDurability *)pclMsg->cData;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( pclInfo->clFishingToolInfo[ i ].siPos > 0 )
		{
			pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->clFishingToolInfo[ i ].siPos ].Set( &pclInfo->clFishingToolInfo[ i ].clItem );
		}
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTS(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FishingGetMarineProducts *pclInfo = (cltGameMsgResponse_FishingGetMarineProducts *)pclMsg->cData;
	
	if ( pclInfo->siReason == FISHING_SUCCESS )
	{
		if ( pclInfo->siInvPos > 0 )
		{
			pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siInvPos ].Set( &pclInfo->clItem );
		}
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FishingGetMarineProductsNotify *pclInfo = (cltGameMsgResponse_FishingGetMarineProductsNotify*)pclMsg->cData;

	if ( pclInfo->siCharUnique < 1 )
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

	TCHAR buffer[256];

	if ( pclInfo->siReason == FISHING_SUCCESS )
	{
		TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siFishItemUnique );

		if ( ItemName == NULL || _tcslen( ItemName ) < 1 )
		{
			return;
		}

		PushEffect( GetUniqueFromHash(TEXT("SOUND_EFFECT_FISHING_SUCCESS")), pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY() );
		TCHAR num[128]= TEXT("") ;
		StringCchPrintf(num, 128, TEXT("%d"),pclInfo->siFishNum);
		TCHAR* pText = GetTxtFromMgr(1264);

		Nsprintf(buffer,pText, TEXT("fish"),  ItemName, TEXT("num"), num, NULL );		
		pclCharClient->pclDrawChat->Set( pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );
		
		// 어획정보 창에 어떤 물고기를 몇마리 낚았는지 표시해준다. (PCK : 08.03.26)
		// 나한테만 뿌려준다.
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			if(pclCM->CR[1]->GetCharUnique() == pclInfo->siCharUnique)
			{
				if(m_pDialog[NFISHING_DLG ]!=NULL)
				{
					((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->SetFishingInfo(ItemName, pclInfo->siFishNum);
					((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->m_dwFishiingEndClock = pclClient->CurrentClock;
				}
			}
		}
	}
	else if ( pclInfo->siReason == FISHING_SKILL_LEVEL_LOW )
	{
		if ( pclInfo->siFishItemUnique < 1 )
		{
			return;
		}

		TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siFishItemUnique );

		if ( ItemName == NULL || _tcslen( ItemName ) < 1 )
		{
			return;
		}
		TCHAR* pText = GetTxtFromMgr(458);

		StringCchPrintf(buffer, 256, pText, ItemName);

		pclCharClient->pclDrawChat->Set(pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );
	}
	else
	{
		if ( pclInfo->siFishItemUnique < 1 )
		{
			return;
		}

		TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siFishItemUnique );

		if ( ItemName == NULL || _tcslen( ItemName ) < 1 )
		{
			return;
		}
		TCHAR* pText = GetTxtFromMgr(1265);

		StringCchPrintf(buffer, 256, pText, ItemName);
		pclCharClient->pclDrawChat->Set(pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );

		// 실패시에도 어획정보창에 보이게 한다. (PCK : 08.04.21)
		// 나한테만 뿌려준다.
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			if(pclCM->CR[1]->GetCharUnique() == pclInfo->siCharUnique)
			{
				if(m_pDialog[NFISHING_DLG ]!=NULL)
				{
					((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->ShowFishinginfo();
					((CNFishingDlg*)m_pDialog[NFISHING_DLG ])->m_dwFishiingEndClock = pclClient->CurrentClock;
				}
			}
		}
	}

	return;
}