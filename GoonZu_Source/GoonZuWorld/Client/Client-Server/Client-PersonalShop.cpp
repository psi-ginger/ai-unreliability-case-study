#include "../Client.h"
#include "../Music/Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-PersonalShop.h"

#include "../Common/Map/MapManager/MapManager.h"
#include "../Common/Map/MapManager/MapManager.h"
#include "Char/CharManager/CharManager.h"


#include "../Interface/DrawChat/DrawChat.h"
#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
//#include "../Interface/PersonalShop/PersonalShopDlg.h"
#include "../Interface/PersonalShopSearchDlg/PersonalShopSearchDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../NInterface/NPersonalShopDlg/NPersonalShopDlg.h"
#include "../NInterface/NPersonalShopSearchDlg/NPersonalShopSearchDlg.h"
#include "../ninterface/NManufactureDlg/NManufactureDlg.h"
#include "../NInterface/NLetterDlg/NLetterDlg.h"
#include "../ninterface/NManufactureDlg_New/NManufactureDlg_New.h"
#include "../ninterface/NManufactureSerchDlg/NManufactureSerchDlg.h"

// 특정한 사용자의 개인상점 배너를 받는다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_PERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PersonalShopBanner* pclinfo = (cltGameMsgResponse_PersonalShopBanner*)pclMsg->cData;

	// 말풍선을 띄운다. 
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		if(pclinfo->siResult > 0)
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = true; 
			pclChar->bPremiumPersonalShop = pclinfo->clShopBanner.m_bPremium;
			pclChar->siShopMode			 = pclinfo->clShopBanner.m_siShopMode;
		

			pclinfo->clShopBanner.MakeBannerText(pclItemManager, pclChar->szPersonalShopText, pclChar->szPersonalShopTitle);

			SI16 BannerType = PERSONALSHOP_BANNER;

			if(pclChar->bPremiumPersonalShop)	
			{
				pclChar->pclDrawChat->Set(pclDrawChatSurface, pclChar->szPersonalShopText, true
					, PERSONALSHOP_BANNER_PREMIUM , pclChar->siShopMode, pclChar->szPersonalShopTitle);
			}
			else
			{
				pclChar->pclDrawChat->Set(pclDrawChatSurface, pclChar->szPersonalShopText, true
					, PERSONALSHOP_BANNER, pclChar->siShopMode);				
			}
			
		}
		else
		{
			pclChar->pclDrawChat->StopDraw();
			pclChar->bPersonalShopSwitch = false; 
			pclChar->bPremiumPersonalShop = false;
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

		}
	}
}

// 특정한 사용자의 개인상점 정보를 받는다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_PERSONALSHOPDATA(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PersonalShopData* pclinfo = (cltGameMsgResponse_PersonalShopData*)pclMsg->cData;

	// ShopData를 설정한다.

	if( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_SELL || pclinfo->clShopData.m_siShopMode == PSHOP_MODE_BUY  )
	{
		if( m_pDialog[ NMANUFACTURE_DLG ] )
			m_pDialog[ NMANUFACTURE_DLG ]->DeleteDialog();	

		if( m_pDialog[ NPERSONALSHOP_DLG ] == NULL )
			CreateInterface( NPERSONALSHOP_DLG );
		((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
		((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_bShopDataChanged = true;
		id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
		if(pclCM->IsValidID(id))
		{
			cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
			StringCchCopy(((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
			((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, (TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );
		}
	}
	else if(pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM 
		|| pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
	{
		if ( IsCountrySwitch( Switch_Change_manufacture_request_interface )) 
		{
			if( m_pDialog[ NMANUFACTURE_NEW_DLG ] )
				m_pDialog[ NMANUFACTURE_NEW_DLG ]->DeleteDialog();

			if( m_pDialog[ NMANUFACTURE_NEW_DLG ] == NULL )
				CreateInterface( NMANUFACTURE_NEW_DLG );
			((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
			((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_bShopDataChanged = true;
			id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

			if(pclCM->IsValidID(id))
			{
				cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
				StringCchCopy(((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
				((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, 
					(TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );
			}

		}
		else
		{
			if( m_pDialog[ NPERSONALSHOP_DLG ] )
				m_pDialog[ NPERSONALSHOP_DLG ]->DeleteDialog();

			if( m_pDialog[ NMANUFACTURE_DLG ] == NULL )
				CreateInterface( NMANUFACTURE_DLG );
			((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
			((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_bShopDataChanged = true;
			id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
			if(pclCM->IsValidID(id))
			{
				cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
				StringCchCopy(((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
				((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, 
					(TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );
			}
		}

	}
}
// 개인상점을 시작 하는것을 설정한다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_STARTPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StartPersonalShop* pclinfo = (cltGameMsgResponse_StartPersonalShop*)pclMsg->cData;

	// 말풍선을 제거하도록 한다.
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	// 자기자신에 대한 정보라면 
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		if(pclinfo->siResult > 0)	
		{
			pclChar->bPersonalShopSwitch = true;

			TCHAR* pTitle = GetTxtFromMgr(1343);
			TCHAR* pText	=	NULL	;
			// 제작- 제조 의뢰 일경우는 내용이 다르다 
			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM )// 제조의뢰
			{
				pText = GetTxtFromMgr(40835);
			}
			else if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )	// 제작의뢰 
			{
				pText = GetTxtFromMgr(40836);
			}
			else
			{
				pText = GetTxtFromMgr(1344);
			}		
            if(pclinfo->bPremium)
			{
				pText = GetTxtFromMgr(6631);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		}
		else if(pclinfo->siResult == 0)	
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			TCHAR* pTitle = GetTxtFromMgr(1345);
			TCHAR* pText = GetTxtFromMgr(1346);

			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				pText = GetTxtFromMgr(40871);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		}
		else if(pclinfo->siResult == -1)	
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			TCHAR* pTitle = GetTxtFromMgr(1345);
			TCHAR* pText = GetTxtFromMgr(1347);
			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				pText = GetTxtFromMgr(40872);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		}
		else if(pclinfo->siResult == -2)	
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			TCHAR* pTitle = GetTxtFromMgr(1345);
			TCHAR* pText = GetTxtFromMgr(1348);

			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				pText = GetTxtFromMgr(40873);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		}
		else if(pclinfo->siResult == -3)	
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			TCHAR* pTitle = GetTxtFromMgr(1345);
			TCHAR* pText = GetTxtFromMgr(1349);

			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				pText = GetTxtFromMgr(40874);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		}
		else if(pclinfo->siResult == -4)	
		{
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			TCHAR* pTitle = GetTxtFromMgr(1345);
			TCHAR* pText = GetTxtFromMgr(1350);
			
			if( pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
			{
				pText = GetTxtFromMgr(40875);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
	}

}


// 개인상점을 중지시킨다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_STOPPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StopPersonalShop* pclinfo = (cltGameMsgResponse_StopPersonalShop*)pclMsg->cData;

	// 말풍선을 제거하도록 한다.
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	//
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		if(pclinfo->siResult > 0)
		{
			switch(pclinfo->siResult)
			{
			case 1:	// 사용자가 종료시켰음 
				{
					if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
					{
						if(id == 1)
						{
						}
						else if(pclinfo->siPersonID == ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_siShopperPersonID) // 내가 보고 있는 창이 었다면 
						{		
							TCHAR* pTitle = GetTxtFromMgr(1351);
							TCHAR* pText = GetTxtFromMgr(1352);

							m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						}
					}
				}
				break;
			case 2:	// 물건이 다 팔려서 종료 시켰음 
				{
					if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
					{
						if(id == 1)
						{
							if(pclChar->siShopMode == PSHOP_MODE_SELL)
							{
								TCHAR* pText = GetTxtFromMgr(5766);

								//m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
								PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

								if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
								{
									CreateInterface( NPERSONALSHOP_POP_DLG);	
								}
								
								
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(pText,5765);
								
							}
							if(pclChar->siShopMode == PSHOP_MODE_BUY)	
							{
								
								TCHAR* pText = GetTxtFromMgr(5768);

								PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

								if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
								{
									CreateInterface( NPERSONALSHOP_POP_DLG);	
							
								}
								
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(pText,5767);
								
							}
						}
						else if(pclinfo->siPersonID == ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_siShopperPersonID) // 내가 보고 있는 창이 었다면 
						{			
							if(pclChar->siShopMode == PSHOP_MODE_SELL)
							{
								//TCHAR* pTitle = GetTxtFromMgr(5767);
								TCHAR* pText = GetTxtFromMgr(5768);

								PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

								if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
								{
									CreateInterface( NPERSONALSHOP_POP_DLG);		
									
								}
								
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(pText,5767);
							}
							if(pclChar->siShopMode == PSHOP_MODE_BUY)
							{

								//TCHAR* pTitle = GetTxtFromMgr(5765);
								TCHAR* pText = GetTxtFromMgr(5766);
								PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

								if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
								{
									CreateInterface( NPERSONALSHOP_POP_DLG);			
									
								}
								
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(pText,5765);
								
							}
						}
					}
				}
				break;
			case 3: // 더이상 진행할수 없어서 종료시켰음 
				{
					if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
					{
						if(id == 1)
						{
							if(pclChar->siShopMode == PSHOP_MODE_SELL)	
							{
								TCHAR* pTitle = GetTxtFromMgr(1351);
								TCHAR* pText = GetTxtFromMgr(1359);

								m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							}
							if(pclChar->siShopMode == PSHOP_MODE_BUY)
							{
								TCHAR* pTitle = GetTxtFromMgr(1351);
								TCHAR* pText = GetTxtFromMgr(1360);

								m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							}
						}
						else if(pclinfo->siPersonID == ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_siShopperPersonID) // 내가 보고 있는 창이 었다면 
						{			
							if(pclChar->siShopMode == PSHOP_MODE_SELL)
							{
								TCHAR* pTitle = GetTxtFromMgr(1351);
								TCHAR* pText = GetTxtFromMgr(1359);

								m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							}
							if(pclChar->siShopMode == PSHOP_MODE_BUY)
							{
								TCHAR* pTitle = GetTxtFromMgr(1351);
								TCHAR* pText = GetTxtFromMgr(1360);

								m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							}
						}
					}
				}
				break;
			}

			pclChar->pclDrawChat->StopDraw();
			//=========================================
			// Personal Shop을 설정한다.
			//=========================================
			pclChar->bPersonalShopSwitch = false; 
			pclChar->bPremiumPersonalShop = false;
			StringCchCopy(pclChar->szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
			StringCchCopy(pclChar->szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));

			if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
			{
				// 같은 사람의 정보라면 창을 닫는다.
				if(pclinfo->siPersonID == ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_siShopperPersonID)
				{
					if(((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->IsCreate())
					{
						m_pDialog[ NPERSONALSHOP_DLG ]->DeleteDialog();
					//	CreateInterface( NPERSONALSHOP_DLG );
					}
				}
			}
			if(IsCountrySwitch(Switch_Change_manufacture_request_interface) )	// 상점을 중지 하면서 제조/제작 의뢰 창이 떠있으면 종료 시킨다
			{
				if (m_pDialog[ NMANUFACTURE_NEW_DLG ] != NULL )
				{
					if (m_pDialog[ NMANUFACTURE_NEW_DLG ]->IsCreate() )
					{
						m_pDialog[ NMANUFACTURE_NEW_DLG ]->DeleteDialog();
					}
				}
			}

			pclChar->siShopMode = 0;
		}
	}

}

// 나의 갱신된 개인상점 배너 성공을 받는다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_UPDATEPERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_UpdatePersonalShopBanner* pclinfo = (cltGameMsgResponse_UpdatePersonalShopBanner*)pclMsg->cData;


	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];

		if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
		{
			if(pclinfo->siResult == 0)	CreateInterface( NPERSONALSHOP_DLG );//((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->Hide();
		}

		/*
		if(pclinfo->siResult > 0)
		{
		//=========================================
		// Personal Shop을 설정한다.
		//=========================================
		pclChar->bPersonalShopSwitch = true; 
		pclChar->siShopMode			 = pclinfo->clShopBanner.m_siShopMode;
		pclinfo->clShopBanner.MakeBannerText(pclItemManager, pclChar->szPersonalShopText);

		pclChar->pclDrawChat->Set(pclDrawChatSurface, pclChar->szPersonalShopText, true, PERSONALSHOP_BANNER, pclChar->siShopMode);
		}
		else
		{
		pclChar->pclDrawChat->StopDraw();
		pclChar->bPersonalShopSwitch = false; 
		strcpy(pclChar->szPersonalShopText, TEXT(""));
		}
		*/
	}
}

// 특정한 사용자의 갱신된 개인상점 정보를 받는다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_UpdatePersonalShopData* pclinfo = (cltGameMsgResponse_UpdatePersonalShopData*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

	TCHAR itemname[128] = TEXT("NULL") ;
	if(pclinfo->siItemUnique)
	{
		cltItem clitem;
		clitem.Init();
		clitem.siUnique = pclinfo->siItemUnique;
		clitem.siItemNum= pclinfo->siItemNum;
		if(clitem.GetFullName(pclItemManager, itemname, 128) == false) StringCchCopy(itemname, 128, TEXT(""));
	}



	if ( g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if(pclCM->IsValidID(id))
		{
			if( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_SELL ||  pclinfo->clShopData.m_siShopMode == PSHOP_MODE_BUY )
			{
				if( m_pDialog[ NPERSONALSHOP_DLG ] == NULL)
					CreateInterface( NPERSONALSHOP_DLG );

				((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
				((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_bShopDataChanged = true;
				id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

				cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
				StringCchCopy(((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
				((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, (TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );


				// 같은 사람의 정보라면 ShopData를 설정한다.
				if(pclinfo->clShopData.m_siPersonID == ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_siShopperPersonID)
				{
					// 상점 설정이 이상하면 닫아버린다. => 없애버린다.
					if(pclinfo->clShopData.m_siShopMode == PSHOP_MODE_NONE)
					{
						//((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->Hide();
						if(((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->IsCreate())
							CreateInterface( NPERSONALSHOP_DLG );
					}
					else
					{


						((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
						((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_bShopDataChanged = true;

						pclChar->clMyShopData.Set(&pclinfo->clShopData);

					}

					// 성공 구매, 판매 정보 업데이트
					
					// 제조 의뢰 성공이면 메시지 없다 그냥끝냄
					if( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM || pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
						return;
		

					if(pclinfo->bBuyer)
					{
						TCHAR szMoney[128]= { '\0', };
						g_JYLibFunction.SetNumUnit( pclinfo->siItemCoast, szMoney, sizeof(szMoney) );

						NTCHARString128 strMessage( GetTxtFromMgr(5763) );

						strMessage.Replace( TEXT("#itemname#"), itemname );
						strMessage.Replace( TEXT("#itemnum#"), SI32ToString(pclinfo->siItemNum) );
						strMessage.Replace( TEXT("#money#"), szMoney );

						{
							PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

							if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
							{
								CreateInterface( NPERSONALSHOP_POP_DLG);
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->MoveDialog(5,20);
							}
							//((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->SetTitle(pTitle);
							
							((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(strMessage,5764);
						}
					}
					else
					{
						TCHAR szMoney[128]= { '\0', };
						g_JYLibFunction.SetNumUnit( pclinfo->siItemCoast, szMoney, sizeof(szMoney) );

						NTCHARString128 strMessage( GetTxtFromMgr(5761) );
						
						strMessage.Replace( TEXT("#itemname#"), itemname );
						strMessage.Replace( TEXT("#itemnum#"), SI32ToString(pclinfo->siItemNum) );
						strMessage.Replace( TEXT("#money#"), szMoney );

						{
							PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_LETTER")), 0, 0);

							if((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ]==NULL)
							{
								CreateInterface( NPERSONALSHOP_POP_DLG);	
								((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->MoveDialog(5,20);
							}
							((CNLetterDlg*)m_pDialog[ NPERSONALSHOP_POP_DLG ])->Push(strMessage,5762);
						}

						//m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
					}

				}
			}
			else if( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM )
			{
				if(IsCountrySwitch(Switch_Change_manufacture_request_interface) )
				{	
					if( pclinfo->clShopData.m_clItem[0].siItemNum	>	0	)
					{
						if( m_pDialog[ NMANUFACTURE_NEW_DLG ] == NULL)
						{
							CreateInterface( NMANUFACTURE_NEW_DLG );						
						}

						((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
						((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->RefreshShowData();
						id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

						cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
						StringCchCopy(((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
						((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, 
							(TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );
						// 같은 사람의 정보라면 ShopData를 설정한다.
						if(pclinfo->clShopData.m_siPersonID == ((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_siShopperPersonID)
						{
							// 상점 설정이 이상하면 닫아버린다. => 없애버린다.
							if(pclinfo->clShopData.m_siShopMode == PSHOP_MODE_NONE)
							{
								if(((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->IsCreate())
									CreateInterface( NMANUFACTURE_NEW_DLG );
							}
							else
							{
								((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
								((CNManufactureDlg_New*)(m_pDialog[ NMANUFACTURE_NEW_DLG ]))->RefreshShowData();
								pclChar->clMyShopData.Set(&pclinfo->clShopData);
							}
							// 성공 구매, 판매 정보 업데이트
						}

					}
				}
				else
				{
					if( m_pDialog[ NMANUFACTURE_DLG ] == NULL)
						CreateInterface( NMANUFACTURE_DLG );

					((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
					((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->RefreshShowData();
					id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];

					cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
					StringCchCopy(((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_szShopBanner, MAX_PERSONAL_SHOP_TEXT_SIZE, pclChar->szPersonalShopText);
					((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->Show( pclinfo->siCharUnique, pclinfo->clShopData.m_siPersonID, pclinfo->clShopData.m_siShopMode, (TCHAR*)pclCM->CR[id]->GetName(), pclinfo->clShopData.m_szAccountID, pclChar->bPersonalShopSwitch );


					// 같은 사람의 정보라면 ShopData를 설정한다.
					if(pclinfo->clShopData.m_siPersonID == ((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_siShopperPersonID)
					{
						// 상점 설정이 이상하면 닫아버린다. => 없애버린다.
						if(pclinfo->clShopData.m_siShopMode == PSHOP_MODE_NONE)
						{
							//((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->Hide();
							if(((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->IsCreate())
								CreateInterface( NMANUFACTURE_DLG );
						}
						else
						{
							((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_clShopData.Set(&pclinfo->clShopData);
							((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->RefreshShowData();
							pclChar->clMyShopData.Set(&pclinfo->clShopData);
						}

						// 성공 구매, 판매 정보 업데이트
					}

				}
			}
			else if( pclinfo->clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT ) //
			{
				if(IsCountrySwitch(Switch_Change_manufacture_request_interface) )
				{	
					cltClient *pclclient = ( cltClient * )pclClient;

					cltGameMsgRequest_StopPersonalShop clshopdata( pclinfo->siCharUnique);
					cltMsg clMsg( GAMEMSG_REQUEST_STOPPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
					
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					// 국가코드 안열렸으면 일러 들어오면 안됨 
				}
			}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BUYPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BuyPersonalShop* pclinfo = (cltGameMsgResponse_BuyPersonalShop*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
		TCHAR itemname[128] = TEXT("") ;
		if(pclinfo->siitemUnique)
		{
			cltItem clitem;
			clitem.Init();
			clitem.siUnique = pclinfo->siitemUnique;
			clitem.siItemNum= pclinfo->siItemNum;
			if(clitem.GetFullName(pclItemManager, itemname, 128) == false) StringCchCopy(itemname, 128, TEXT(""));
		}

		if(pclChar)
		{
			TCHAR buffer[128];			
			TCHAR* pText = NULL;

			if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
			{
				TCHAR item[128]=TEXT("");
				TCHAR num[128]=TEXT("");
				StringCchPrintf(num, 128, TEXT("%d"),pclinfo->siItemNum);

				switch(pclinfo->siResult )
				{
				case BUYPERSONALSHOP_SUCCESS_BUY:			pText = GetTxtFromMgr(1361); Nsprintf(buffer, pText, TEXT("#item#"), itemname, TEXT("#num#"), num, NULL);	break;
				case BUYPERSONALSHOP_FAIL_UNKNOWN:			pText = GetTxtFromMgr(1362); StringCchPrintf(buffer, 128, pText);										break;
				case BUYPERSONALSHOP_FAIL_SHOPPERSTATUS:	pText = GetTxtFromMgr(1363); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);break;
				case BUYPERSONALSHOP_FAIL_BUYERSTATUS:		pText = GetTxtFromMgr(1364); StringCchPrintf(buffer, 128, pText);										break;	
				case BUYPERSONALSHOP_FAIL_SHOWCASESTATUS:	pText = GetTxtFromMgr(1365); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);	break;
				case BUYPERSONALSHOP_FAIL_BUY_NOITEM:		pText = GetTxtFromMgr(1366); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);	break;	
				case BUYPERSONALSHOP_FAIL_BUY_NOMONEY:		pText = GetTxtFromMgr(1367); StringCchPrintf(buffer, 128, pText);			break;
				case BUYPERSONALSHOP_FAIL_BUY_NOINVSPACE:	pText = GetTxtFromMgr(1368); StringCchPrintf(buffer, 128, pText);		break;	
				default:									StringCchPrintf(buffer, 128, TEXT("")); break;
				}
			}

			//TODO: 아이템 성공 구매 메시지 팝

			//TCHAR buffer[255];


		}
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SELLPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SellPersonalShop* pclinfo = (cltGameMsgResponse_SellPersonalShop*)pclMsg->cData;

	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
		TCHAR itemname[128] = TEXT("") ;
		if(pclinfo->siitemUnique)
		{
			cltItem clitem;
			clitem.Init();
			clitem.siUnique = pclinfo->siitemUnique;
			clitem.siItemNum= pclinfo->siItemNum;
			if(clitem.GetFullName(pclItemManager, itemname, 128) == false) StringCchCopy(itemname, 128, TEXT(""));
		}

		if(pclChar)
		{
			TCHAR buffer[128];
			TCHAR* pText = NULL;

			if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ])))
			{
				switch(pclinfo->siResult )
				{
				case SELLPERSONALSHOP_SUCCESS_SELL:			
					{
						pText = GetTxtFromMgr(/*1361*/3771); 
						TCHAR Nbuffer[512] = TEXT("");

						TCHAR num[128] = TEXT("") ;

						StringCchPrintf( num, 128, TEXT("%d"), pclinfo->siItemNum );
						Nsprintf( buffer,pText, TEXT("item"),  itemname, TEXT("num"), num, NULL );
						

					}
					break;
				case SELLPERSONALSHOP_FAIL_UNKNOWN:			pText = GetTxtFromMgr(1362); StringCchPrintf(buffer, 128, pText);										break;
				case SELLPERSONALSHOP_FAIL_SHOPPERSTATUS:	pText = GetTxtFromMgr(1363); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);break;
				case SELLPERSONALSHOP_FAIL_SELLERSTATUS:	pText = GetTxtFromMgr(1364); StringCchPrintf(buffer, 128, pText);										break;	
				case SELLPERSONALSHOP_FAIL_SHOWCASESTATUS:	pText = GetTxtFromMgr(1365); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);	break;
				case SELLPERSONALSHOP_FAIL_SELL_NOITEM:		pText = GetTxtFromMgr(1366); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);	break;	
				case SELLPERSONALSHOP_FAIL_SELL_NOMONEY:	pText = GetTxtFromMgr(1367); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);		break;
				case SELLPERSONALSHOP_FAIL_SELL_NOINVSPACE:	pText = GetTxtFromMgr(1368); StringCchPrintf(buffer, 128, pText, ((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->m_szCharName);	break;	
				default:									StringCchPrintf(buffer, 128, TEXT("")); break;
				}

				((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->DisplayMessage(pclinfo->siResult, buffer);
			}
		}
	}
}



void cltClient::DoMsg_GAMEMSG_RESPONSE_SEARCHPERSONALSHOPITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SearchPersonalShopItem* pclinfo = (cltGameMsgResponse_SearchPersonalShopItem*)pclMsg->cData;

	if(pclCM->IsValidID(1))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[1];
		if(pclinfo->siResult > 0)
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				// Data를 설정하고 Data가 갱신되었다는 것을 알려줌 
				((CNPersonalShopSearchDlg*)(m_pDialog[ NPERSONALSHOPSEARCH_DLG ]))->m_clSearhShopData.Set(&pclinfo->clSearchShopData);
				((CNPersonalShopSearchDlg*)(m_pDialog[ NPERSONALSHOPSEARCH_DLG ]))->m_bShopDataChanged = true;
			}
		}
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_SEARCHMANUFACTURE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SearchManufacture* pclinfo = (cltGameMsgResponse_SearchManufacture*)pclMsg->cData;

	if(pclCM->IsValidID(1))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[1];
		if(pclinfo->siResult > 0)
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				// Data를 설정하고 Data가 갱신되었다는 것을 알려줌 
				((CNManufactureSearchDlg*)(m_pDialog[ MANUFACTURESEARCH_DLG ]))->m_clSearhShopData.Set(&pclinfo->clSearchShopData);
				((CNManufactureSearchDlg*)(m_pDialog[ MANUFACTURESEARCH_DLG ]))->m_bShopDataChanged = true;
			}
		}
	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_MAKEPERSONALSHOP(cltMsg* pclMsg, SI32 id)
{
 	cltGameMsgResponse_MakePersonalShop* pclinfo = (cltGameMsgResponse_MakePersonalShop *)pclMsg->cData;
	id = cltCharCommon::CharUniqueInfo[pclinfo->siCharUnique];
	if(pclCM->IsValidID(id))
	{
		cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
		TCHAR itemname[128] = TEXT("") ;
		if(pclinfo->siitemUnique)
		{
			cltItem clitem;
			clitem.Init();
			clitem.siUnique = pclinfo->siitemUnique;
			clitem.siItemNum= pclinfo->siItemNum;
			if(clitem.GetFullName(pclItemManager, itemname, 128) == false) StringCchCopy(itemname, 128, TEXT(""));
		}

		if(pclChar)
		{
			TCHAR buffer[128];
			TCHAR* pText = NULL;
			TCHAR* pTitle =	NULL	;
			if ( pclClient->IsCountrySwitch( Switch_Change_manufacture_request_interface ) ) //제조 인터페이스 개선이 들어가면 제조창에 띄울수가 없다 그래서 메세지 창으로 수정 
			{
				pTitle = GetTxtFromMgr(416);	// 기본적으로 실패 메세지 

				switch( pclinfo->siResult  ) 
				{
				case MAKEPERSONALSHOP_SUCCESS_MAKE :
					{
						pTitle	=	GetTxtFromMgr(389)	;//[%s] %d개가 제작되었습니다.
						pText	=	GetTxtFromMgr(1369); 
						StringCchPrintf(buffer, 128, pText,itemname, pclinfo->siItemNum);
					}
					break;
				case MAKEPERSONALSHOP_FAIL_UNKNOWN://물품을 제작하지 못하였습니다.
					{
						pText = GetTxtFromMgr(1370); 
						StringCchPrintf(buffer, 128, pText);
					}break;
				case MAKEPERSONALSHOP_FAIL_SHOPPERSTATUS:// 제작/제조 의뢰자의 의뢰정보가 변경되었습니다
				case MAKEPERSONALSHOP_FAIL_SHOWCASESTATUS:
					{
						pText = GetTxtFromMgr(40844); 
						StringCchPrintf(buffer, 128, pText);			

					}break;
				case MAKEPERSONALSHOP_FAIL_MAKERSTATUS:
					{
						pText = GetTxtFromMgr(1371); 
						StringCchPrintf(buffer, 128, pText);

					}break;
				case MAKEPERSONALSHOP_FAIL_MAKE_NOITEM:
					{
						pText = GetTxtFromMgr(40845); 
						StringCchPrintf(buffer, 128, pText);
					}break;
				case MAKEPERSONALSHOP_FAIL_MAKE_NOMONEY:
					{
						pText = GetTxtFromMgr(40846); 
						StringCchPrintf(buffer, 128, pText);
					}break;
				case MAKEPERSONALSHOP_FAIL_MAKE_NEEDABILITY:
					{
						pText = GetTxtFromMgr(40847); 
						StringCchPrintf(buffer, 128, pText);
					}break;
				case MAKEPERSONALSHOP_FAIL_NOWMAKINGSTATUS:
					{
						pText = GetTxtFromMgr(40839); 
						StringCchPrintf(buffer, 128, pText);

					}break;

				}
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer)	;
			}
			else
			{
				if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ])))
				{
					switch(pclinfo->siResult )
					{
					case MAKEPERSONALSHOP_SUCCESS_MAKE:			pText = GetTxtFromMgr(1369); StringCchPrintf(buffer, 128, pText,				itemname, pclinfo->siItemNum);		break;
					case MAKEPERSONALSHOP_FAIL_UNKNOWN:			pText = GetTxtFromMgr(1370); StringCchPrintf(buffer, 128, pText);												break;
					case MAKEPERSONALSHOP_FAIL_SHOPPERSTATUS:	pText = GetTxtFromMgr(1363); StringCchPrintf(buffer, 128, pText,		((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_szCharName);	break;
					case MAKEPERSONALSHOP_FAIL_MAKERSTATUS:		pText = GetTxtFromMgr(1371); StringCchPrintf(buffer, 128, pText);											break;	
					case MAKEPERSONALSHOP_FAIL_SHOWCASESTATUS:	pText = GetTxtFromMgr(1365); StringCchPrintf(buffer, 128, pText,((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_szCharName);	break;
					case MAKEPERSONALSHOP_FAIL_MAKE_NOITEM:		pText = GetTxtFromMgr(1372); StringCchPrintf(buffer, 128, pText, ((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->m_szCharName);	break;	
					case MAKEPERSONALSHOP_FAIL_MAKE_NOMONEY:	pText = GetTxtFromMgr(1373); StringCchPrintf(buffer, 128, pText);		break;
					case MAKEPERSONALSHOP_FAIL_MAKE_NEEDABILITY:pText = GetTxtFromMgr(1374); StringCchPrintf(buffer, 128, pText);	break;					
					default:									StringCchPrintf(buffer, 128, TEXT("")); break;
					}				
					//m_pDialog[NOTIFYMSG_DLG]->SetText()
					bool bSuccess = true;

					if(pclinfo->siResult < 1)
					{
						bSuccess = false;
					}				

					((CNManufactureDlg*)(m_pDialog[ NMANUFACTURE_DLG ]))->DisplayMessage(bSuccess,buffer);
					//((CNPersonalShopDlg*)(m_pDialog[ NPERSONALSHOP_DLG ]))->DisplayMessage(pclinfo->siResult, buffer);
				}
				else
				{
					if(pclinfo->siResult ==MAKEPERSONALSHOP_FAIL_NOWMAKINGSTATUS )// 제조의뢰시는 다이얼로그가 떠있지 않다 . 그러므로 메세지를 띄워준다 
					{
						pTitle = GetTxtFromMgr(416);
						pText = GetTxtFromMgr(40839); 
						m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText)	;
					}
				}
			}
		}
	}
}


